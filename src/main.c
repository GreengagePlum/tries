/**
 * @file main.c
 * @author Efe ERKEN (efe.erken@etu.sorbonne-universite.fr)
 * @brief Fichier d'entrée dans le programme contenant la logique haut niveau des opérations
 * @version 0.3
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
        if (argc != 4)
        {
            fprintf(stderr, "usage: %s %d <TrieType> <isRebalanced>", argv[0], INSERER);
            exit(1);
        }
        if (tt == PATRICIA)
            insererPT();
        else if (tt == HYBRID)
        {
            int isRebalanced = atoi(argv[3]);
            insererTH(isRebalanced);
        }
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
        if (argc != 5)
        {
            fprintf(stderr, "usage: %s %d <TrieType> <arbre1.json> <arbre2.json>", argv[0], FUSION);
            exit(1);
        }
        if (tt == PATRICIA)
            fusionMainPT(argv[3], argv[4]);
        else if (tt == HYBRID)
            fusionMainTH(argv[3], argv[4]);
        else
        {
            fprintf(stderr, "Erreur, TrieType [%d] inconnu", tt);
            exit(1);
        }
        break;
    case LISTEMOTS:
        if (argc != 4)
        {
            fprintf(stderr, "usage: %s %d <TrieType> <arbre.json>", argv[0], LISTEMOTS);
            exit(1);
        }
        if (tt == PATRICIA)
            listeMotsMainPT(argv[3]);
        else if (tt == HYBRID)
            listeMotsMainTH(argv[3]);
        else
        {
            fprintf(stderr, "Erreur, TrieType [%d] inconnu", tt);
            exit(1);
        }
        break;
    case PROFONDEURMOYENNE:
        if (argc != 4)
        {
            fprintf(stderr, "usage: %s %d <TrieType> <arbre.json>", argv[0], PROFONDEURMOYENNE);
            exit(1);
        }
        if (tt == PATRICIA)
            profondeurMoyenneMainPT(argv[3]);
        else if (tt == HYBRID)
            profondeurMoyenneMainTH(argv[3]);
        else
        {
            fprintf(stderr, "Erreur, TrieType [%d] inconnu", tt);
            exit(1);
        }
        break;
    case PREFIXE:
        if (argc != 5)
        {
            fprintf(stderr, "usage: %s %d <TrieType> <arbre.json> <word>", argv[0], PREFIXE);
            exit(1);
        }
        if (tt == PATRICIA)
            prefixeMainPT(argv[3], argv[4]);
        else if (tt == HYBRID)
            prefixeMainTH(argv[3], argv[4]);
        else
        {
            fprintf(stderr, "Erreur, TrieType [%d] inconnu", tt);
            exit(1);
        }
        break;
    default:
        fprintf(stderr, "Erreur, OpType [%d] inconnu", op);
        exit(1);
    }

    return EXIT_SUCCESS;
}
