/**
 * @file helpers.c
 * @author Efe ERKEN (efe.erken@etu.sorbonne-universite.fr)
 * @brief Fichier source contenant les corps des fonctions pour gérer l'interface utilisateur
 * @version 0.2
 * @date 2024-12-05
 *
 * @copyright Copyright (C) 2024 Efe ERKEN
 *
 */

#define _POSIX_C_SOURCE 200809L
#include "helpers.h"
#include "hybrid.h"
#include "patricia.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void insererPT(void)
{
    fprintf(stderr, "Fonctionnalité pas encore implanté");
    exit(1);
}

void insererTH(void)
{
    TrieHybride *th = newTH();
    long count = 1;
    ssize_t sz;
    size_t cap = 0;
    char *s = NULL;
    while ((sz = getline(&s, &cap, stdin)) > 0)
    {
        if (!feof(stdin))
            s[sz - 1] = '\0';
        th = ajoutTH(th, s, count++);
    }
    if (sz == -1 && ferror(stdin))
    {
        fprintf(stderr, "Erreur, getline dans insererTH");
        exit(1);
    }
    free(s);
    s = printJSONTH(th);
    printf("%s", s);
    free(s);
    deleteTH(&th);
}

/**
 * @private
 *
 * @brief Lit un fichier JSON dans son ensemble dans un buffer
 *
 * @param [in] f Un pointeur de fichier auparavant ouvert au moins en lecture
 * @param [out] sz Un entier contenant la taille en octets du buffer renvoyé à la fin
 * @return Une chaine de caractères contenant la version sérialisé en JSON du Trie Hybride
 *
 * @pre @a sz n'est pas nul
 * @post C'est à l'appellant de désallouer la chaine retourné
 *
 * Détermine la taille du fichier en question et alloue un buffer de cette taille exactement en une fois. Puis, lit tout
 * le fichier vers le buffer en une fois. Retourne un pointeur vers le buffer qui contient le contenu de tout le
 * fichier. @a sz contient la taille du buffer renvoyé à la fin. Le buffer renvoyé n'est pas forcément nul terminé. Le
 * curseur du fichier est déplacé au début pour lire tout le fichier mais il est remis à sa place d'origine à la sortie
 * de fonction. Il peut donc continuer à être utilisé comme normal après appel à cette fonction.
 *
 */
char *readJSON(FILE *f, size_t *sz)
{
    const long fpos = ftell(f);
    if (fpos == -1)
    {
        perror("Erreur, ftell dans readJSON");
        exit(1);
    }
    if (fseek(f, 0, SEEK_END) == -1)
    {
        perror("Erreur, fseek dans readJSON");
        exit(1);
    }
    const long fsize = ftell(f);
    if (fsize == -1)
    {
        perror("Erreur, ftell dans readJSON");
        exit(1);
    }
    if (fseek(f, 0, SEEK_SET) == -1)
    {
        perror("Erreur, fseek dans readJSON");
        exit(1);
    }

    char *s = malloc(fsize * sizeof(*s));
    if (!s)
    {
        fprintf(stderr, "Erreur, malloc dans readJSON");
        exit(1);
    }
    if (fread(s, fsize, 1, f) < 1)
    {
        fprintf(stderr, "Erreur, fread dans readJSON");
        exit(1);
    }
    if (fseek(f, fpos, SEEK_SET) == -1)
    {
        perror("Erreur, fseek dans readJSON");
        exit(1);
    }
    clearerr(f);
    *sz = fsize;
    return s;
}

void suppressionPT(const char *path)
{
    (void)path;
    fprintf(stderr, "Fonctionnalité pas encore implanté");
    exit(1);
}

void suppressionTH(const char *path)
{
    FILE *f = fopen(path, "r+");
    if (!f)
    {
        perror("Erreur, fopen dans suppressionTH");
        exit(1);
    }

    size_t fsize;
    char *s = readJSON(f, &fsize);
    TrieHybride *th = parseJSONTH(s, fsize);
    free(s);

    ssize_t sz;
    size_t cap = 0;
    s = NULL;
    while ((sz = getline(&s, &cap, stdin)) > 0)
    {
        if (!feof(stdin))
            s[sz - 1] = '\0';
        th = supprTH(th, s);
    }
    if (sz == -1 && ferror(stdin))
    {
        fprintf(stderr, "Erreur, getline dans suppressionTH");
        exit(1);
    }
    free(s);

    rewind(f);
    if (ftruncate(fileno(f), 0) == -1)
    {
        perror("Erreur, ftruncate dans suppressionTH");
        exit(1);
    }
    s = printJSONTH(th);
    fprintf(f, "%s", s);

    if (fclose(f) == EOF)
    {
        perror("Erreur, fclose dans suppressionTH");
        exit(1);
    }
    free(s);
    deleteTH(&th);
}

void listeMotsMainPT(const char *path)
{
    (void)path;
    fprintf(stderr, "Fonctionnalité pas encore implanté");
    exit(1);
}

void listeMotsMainTH(const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f)
    {
        perror("Erreur, fopen dans listeMotsMainTH");
        exit(1);
    }

    size_t fsize;
    char *s = readJSON(f, &fsize);
    TrieHybride *th = parseJSONTH(s, fsize);
    free(s);

    char **tab = listeMotsTH(th);
    if (tab)
    {
        for (char **i = tab; *i; i++)
        {
            printf("%s\n", *i);
        }
    }

    if (fclose(f) == EOF)
    {
        perror("Erreur, fclose dans listeMotsMainTH");
        exit(1);
    }
    deleteListeMotsTH(tab);
    deleteTH(&th);
}

void profondeurMoyenneMainPT(const char *path)
{
    (void)path;
    fprintf(stderr, "Fonctionnalité pas encore implanté");
    exit(1);
}

void profondeurMoyenneMainTH(const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f)
    {
        perror("Erreur, fopen dans profondeurMoyenneMainTH");
        exit(1);
    }

    size_t fsize;
    char *s = readJSON(f, &fsize);
    TrieHybride *th = parseJSONTH(s, fsize);
    free(s);

    printf("%f", profondeurMoyenneTH(th));

    if (fclose(f) == EOF)
    {
        perror("Erreur, fclose dans profondeurMoyenneMainTH");
        exit(1);
    }
    deleteTH(&th);
}

void prefixeMainPT(const char *path, const char *cle)
{
    (void)path;
    (void)cle;
    fprintf(stderr, "Fonctionnalité pas encore implanté");
    exit(1);
}

void prefixeMainTH(const char *path, const char *cle)
{
    FILE *f = fopen(path, "r");
    if (!f)
    {
        perror("Erreur, fopen dans profondeurMoyenneMainTH");
        exit(1);
    }

    size_t fsize;
    char *s = readJSON(f, &fsize);
    TrieHybride *th = parseJSONTH(s, fsize);
    free(s);

    printf("%d", prefixeTH(th, cle));

    if (fclose(f) == EOF)
    {
        perror("Erreur, fclose dans profondeurMoyenneMainTH");
        exit(1);
    }
    deleteTH(&th);
}
