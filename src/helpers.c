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
    if (fseek(f, 0, SEEK_END) == -1)
    {
        perror("Erreur, fseek dans suppressionTH");
        exit(1);
    }
    long fsize = ftell(f);
    if (fsize == -1)
    {
        perror("Erreur, ftell dans suppressionTH");
        exit(1);
    }
    if (fseek(f, 0, SEEK_SET) == -1)
    {
        perror("Erreur, fseek dans suppressionTH");
        exit(1);
    }

    char *s = malloc(fsize * sizeof(*s));
    if (!s)
    {
        fprintf(stderr, "Erreur, malloc dans suppressionTH");
        exit(1);
    }
    if (fread(s, fsize, 1, f) < 1)
    {
        fprintf(stderr, "Erreur, fread dans suppressionTH");
        exit(1);
    }

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
