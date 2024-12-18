#!/bin/bash
set -e

./inserer 0 test/Shakespeare/allswell.txt
./suppression 0 test/Shakespeare/allswell.txt

./inserer 0 test/Shakespeare/macbeth.txt
./suppression 0 test/Shakespeare/macbeth.txt
