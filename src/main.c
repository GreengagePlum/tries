/**
 * @file main.c
 * @author Efe ERKEN (efe.erken@etu.sorbonne-universite.fr)
 * @brief Fichier d'entrée dans le programme contenant la logique haut niveau des opérations
 * @version 0.2
 * @date 2024-11-18
 *
 * @copyright Copyright (C) 2024 Efe ERKEN
 *
 */

#include "helpers.h"
#include <stdio.h>
#include <stdlib.h>

typedef enum
{
    INSERER = 0,
    SUPPRESSION = 1,
    FUSION = 2,
    LISTEMOTS = 3,
    PROFONDEURMOYENNE = 4,
    PREFIXE = 5
} OpType;

typedef enum
{
    PATRICIA = 0,
    HYBRID = 1
} TrieType;

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "usage: %s <OpType> <TrieType> ...", argv[0]);
        exit(1);
    }

    OpType op = atoi(argv[1]);
    TrieType tt = atoi(argv[2]);
    switch (op)
    {
    case INSERER:
        if (argc != 3)
        {
            fprintf(stderr, "usage: %s %d <TrieType>", argv[0], INSERER);
            exit(1);
        }
        if (tt == PATRICIA)
            insererPT();
        else if (tt == HYBRID)
            insererTH();
        else
        {
            fprintf(stderr, "Erreur, TrieType [%d] inconnu", tt);
            exit(1);
        }
        break;
    case SUPPRESSION:
        if (argc != 4)
        {
            fprintf(stderr, "usage: %s %d <TrieType> <inoutfile.json>", argv[0], SUPPRESSION);
            exit(1);
        }
        if (tt == PATRICIA)
            suppressionPT(argv[3]);
        else if (tt == HYBRID)
            suppressionTH(argv[3]);
        else
        {
            fprintf(stderr, "Erreur, TrieType [%d] inconnu", tt);
            exit(1);
        }
        break;
    case FUSION:
        break;
    case LISTEMOTS:
        break;
    case PROFONDEURMOYENNE:
        break;
    case PREFIXE:
        break;
    default:
        fprintf(stderr, "Erreur, OpType [%d] inconnu", op);
        exit(1);
    }

    return EXIT_SUCCESS;
}
