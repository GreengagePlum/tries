#include "cJSON.h"
#include "hybrid.h"
#include "patricia.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    cJSON *cj = cJSON_CreateTrue();
    printf("Exec name: %s\n#Args = %d\nIs cJSON working: %s\nP = %d\nH = %d\n", argv[0], argc,
           cJSON_IsTrue(cj) ? "YES" : "NO", patricia(42), hybrid(42));
    cJSON_free(cj);
    return EXIT_SUCCESS;
}
