#!/bin/bash

# Function to compute user+sys time for deletion
run_deletion_test() {
    local cmd="$1"
    local reinsertion_cmd="$2"
    local runs=100
    local times=()

    echo "Running: $cmd ($runs times, with reinsertion before each run)"

    for i in $(seq 1 $runs); do
        # Reinsert the data before timing the deletion
        $reinsertion_cmd

        # Time the deletion command
        output=$( (time $cmd) 2>&1 | grep -E 'user|sys' )
        user_time=$(echo "$output" | grep "user" | awk '{print $2}' | sed 's/m/:/g;s/s//g' | awk -F':' '{print $1*60 + $2}')
        sys_time=$(echo "$output" | grep "sys" | awk '{print $2}' | sed 's/m/:/g;s/s//g' | awk -F':' '{print $1*60 + $2}')
        total_time=$(echo "$user_time + $sys_time" | bc)
        times+=($total_time)
    done

    # Sort times
    sorted_times=($(printf '%s\n' "${times[@]}" | sort -n))

    # Calculate min, max, median, and average
    min=${sorted_times[0]}
    max=${sorted_times[-1]}
    len=${#sorted_times[@]}
    if (( $len % 2 == 0 )); then
        median=$(echo "(${sorted_times[$len/2-1]} + ${sorted_times[$len/2]}) / 2" | bc -l)
    else
        median=${sorted_times[$len/2]}
    fi
    sum=$(printf '%s\n' "${sorted_times[@]}" | paste -sd+ | bc)
    avg=$(echo "$sum / $len" | bc -l)

    # Print results
    echo "Results for: $cmd"
    echo "Min: $min seconds"
    echo "Max: $max seconds"
    echo "Median: $median seconds"
    echo "Average: $avg seconds"
    echo ""
}

# Run the deletion tests
reinsertion_cmd="./inserer 1 tmp.txt 1"
run_deletion_test "./suppression 1 test/Shakespeare/hamlet.txt" "$reinsertion_cmd"

