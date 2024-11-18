/**
 * @file main.c
 * @author Efe ERKEN (efe.erken@etu.sorbonne-universite.fr)
 * @brief Fichier d'entrée dans le programme contenant la logique haut niveau des opérations
 * @version 0.1
 * @date 2024-11-18
 *
 * @copyright Copyright (C) 2024 Efe ERKEN
 *
 */

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
