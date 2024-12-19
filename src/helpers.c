/**
 * @file helpers.c
 * @author Efe ERKEN (efe.erken@etu.sorbonne-universite.fr)
 * @brief Fichier source contenant les corps des fonctions pour gérer l'interface utilisateur
 * @version 0.3
 * @date 2024-12-05
 *
 * @copyright Copyright (C) 2024 Efe ERKEN
 *
 */

#define _POSIX_C_SOURCE 200809L /* for `getline()` */
#include "helpers.h"
#include "hybrid.h"
#include "patricia.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void insererPT(void)
{
    PatriciaNode *pt = create_patricia_node();
    ssize_t sz;
    size_t cap = 0;
    char *s = NULL;
    int cmp_count = 0;
    while ((sz = getline(&s, &cap, stdin)) > 0)
    {
        if (!feof(stdin))
            s[sz - 1] = '\0';
        insert_patricia_compt(pt, s, &cmp_count);
    }
    if (sz == -1 && ferror(stdin))
    {
        fprintf(stderr, "Erreur, getline dans insererPT");
        exit(1);
    }
    free(s);
    char *json = printJSONPT(pt);
    printf("%s", json);
    free(json);
    fprintf(stderr, "Nombre de comparaisons: %d\n", cmp_count);
    free_patricia_node(pt);
}

void insererTH(void)
{
    TrieHybride *th = newTH();
    long count = 1;
    ssize_t sz;
    size_t cap = 0;
    char *s = NULL;
    int cmp_count = 0;
    while ((sz = getline(&s, &cap, stdin)) > 0)
    {
        if (!feof(stdin))
            s[sz - 1] = '\0';
        fprintf(stderr, "[INSERER] Hauteur: [%ld]\n", hauteurTH(th));
        fprintf(stderr, "[INSERER] Profondeur moyenne: [%f]\n", profondeurMoyenneTH(th));
        th = ajoutTH(th, s, count++, &cmp_count);
        fprintf(stderr, "[INSERER] Nombre de comparaisons de caractères: [%d]\n", cmp_count);
        fprintf(stderr, "[INSERER] Longueur de la clé: [%lu]\n\n", lgueur(s));
        cmp_count = 0;
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
 * @pre @a f doit supporter les déplacements de curseur
 * @pre @a sz n'est pas nul
 * @post C'est à l'appellant de désallouer la chaine retourné
 *
 * Détermine la taille du fichier en question et alloue un buffer de cette taille exactement en une fois. Puis, lit tout
 * le fichier vers le buffer en une fois. Retourne un pointeur vers le buffer qui contient le contenu de tout le
 * fichier. @a sz contient la taille du buffer renvoyé à la fin. Le buffer renvoyé n'est pas forcément nul terminé. Le
 * curseur du fichier est déplacé au début pour lire tout le fichier mais il est remis à sa place d'origine à la sortie
 * de fonction. Les erreurs du flux sont donc forcément effacées dûs à ces déplacements. Il peut donc continuer à être
 * utilisé comme normal après appel à cette fonction.
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
    rewind(f);

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
    FILE *f = fopen(path, "r+");
    if (!f)
    {
        perror("Erreur, fopen dans suppressionPT");
        exit(1);
    }
    size_t fsize;
    char *s = readJSON(f, &fsize);
    PatriciaNode *pt = parseJSONPT(s, fsize);
    free(s);
    int cmp_count = 0;
    ssize_t sz;
    size_t cap = 0;
    s = NULL;
    while ((sz = getline(&s, &cap, stdin)) > 0)
    {
        if (!feof(stdin))
            s[sz - 1] = '\0';
        int i = delete_word(pt, s, &cmp_count);
        if (!(i == 0 || i == 1))
        {
            fprintf(stderr, "Erreur, suppressionPT");
            exit(1);
        }
    }
    if (sz == -1 && ferror(stdin))
    {
        fprintf(stderr, "Erreur, getline dans suppressionPT");
        exit(1);
    }

    free(s);
    rewind(f);
    if (ftruncate(fileno(f), 0) == -1)
    {
        perror("Erreur, ftruncate dans suppressionPT");
        exit(1);
    }
    s = printJSONPT(pt);
    fprintf(f, "%s", s);

    if (fclose(f) == EOF)
    {
        perror("Erreur, fclose dans suppressionPT");
        exit(1);
    }
    free(s);
    fprintf(stderr, "Nombre de comparaisons: %d\n", cmp_count);
    free_patricia_node(pt);
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
    int count = 0;
    while ((sz = getline(&s, &cap, stdin)) > 0)
    {
        if (!feof(stdin))
            s[sz - 1] = '\0';
        fprintf(stderr, "[SUPPRESSION] Hauteur: [%ld]\n", hauteurTH(th));
        fprintf(stderr, "[SUPPRESSION] Profondeur moyenne: [%f]\n", profondeurMoyenneTH(th));
        th = supprTH(th, s, &count);
        fprintf(stderr, "[SUPPRESSION] Nombre de noeuds visités: [%d]\n", count);
        fprintf(stderr, "[SUPPRESSION] Longueur de la clé: [%lu]\n\n", lgueur(s));
        count = 0;
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

void fusionMainPT(const char *path1, const char *path2)
{
    FILE *f1 = fopen(path1, "r");
    if (!f1)
    {
        perror("Erreur, fopen dans fusionMainPT");
        exit(1);
    }
    FILE *f2 = fopen(path2, "r");
    if (!f2)
    {
        perror("Erreur, fopen dans fusionMainPT");
        exit(1);
    }
    size_t fsize;
    char *s;
    s = readJSON(f1, &fsize);
    PatriciaNode *pt1 = parseJSONPT(s, fsize);
    free(s);
    s = readJSON(f2, &fsize);
    PatriciaNode *pt2 = parseJSONPT(s, fsize);
    free(s);

    pt1 = pat_fusion(pt1, pt2);
    s = printJSONPT(pt1);
    printf("%s", s);
    free(s);

    if (fclose(f1) == EOF)
    {
        perror("Erreur, fclose dans profondeurMoyenneMainTH");
        exit(1);
    }
    if (fclose(f2) == EOF)
    {
        perror("Erreur, fclose dans profondeurMoyenneMainTH");
        exit(1);
    }
    free_patricia_node(pt1);
}

void fusionMainTH(const char *path1, const char *path2)
{
    FILE *f1 = fopen(path1, "r");
    if (!f1)
    {
        perror("Erreur, fopen dans profondeurMoyenneMainTH");
        exit(1);
    }
    FILE *f2 = fopen(path2, "r");
    if (!f2)
    {
        perror("Erreur, fopen dans profondeurMoyenneMainTH");
        exit(1);
    }

    size_t fsize;
    char *s;
    s = readJSON(f1, &fsize);
    TrieHybride *th1 = parseJSONTH(s, fsize);
    free(s);
    s = readJSON(f2, &fsize);
    TrieHybride *th2 = parseJSONTH(s, fsize);
    free(s);

    th1 = fusionTH(&th1, th2);
    s = printJSONTH(th1);
    printf("%s", s);
    free(s);

    if (fclose(f1) == EOF)
    {
        perror("Erreur, fclose dans profondeurMoyenneMainTH");
        exit(1);
    }
    if (fclose(f2) == EOF)
    {
        perror("Erreur, fclose dans profondeurMoyenneMainTH");
        exit(1);
    }
    deleteTH(&th1);
    deleteTH(&th2);
}

void listeMotsMainPT(const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f)
    {
        perror("Erreur, fopen dans listeMotsMainPT");
        exit(1);
    }
    size_t fsize;
    int nb_comp = 0;
    int nb_node = 0;
    char *s = readJSON(f, &fsize);
    PatriciaNode *pt = parseJSONPT(s, fsize);
    free(s);
    char** liste = liste_mots_patricia(pt, &nb_node, &nb_comp);
    if (liste)
    {
        for (char **i = liste; *i; i++)
        {
            printf("%s\n", *i);
        }
    }

    if (fclose(f) == EOF)
    {
        perror("Erreur, fclose dans listeMotsMainPT");
        exit(1);
    }
    int* nb_node2 = 0;
    int size = comptage_mots_patricia(pt, nb_node2);
    fprintf(stderr, "Nombre de comparaisons: %d\nNombre de noeuds visités %d\n", nb_comp, nb_node);
    free_patricia_node(pt);
    free_list_patricia(liste, size);
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
    FILE *f = fopen(path, "r");
    if (!f)
    {
        perror("Erreur, fopen dans profondeurMoyenneMainPT");
        exit(1);
    }

    size_t fsize;
    char *s = readJSON(f, &fsize);
    PatriciaNode *pt = parseJSONPT(s, fsize);
    free(s);

    printf("%f", profondeur_moyenne_patricia_feuille(pt));

    if (fclose(f) == EOF)
    {
        perror("Erreur, fclose dans profondeurMoyenneMainPT");
        exit(1);
    }
    free_patricia_node(pt);
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
    FILE *f = fopen(path, "r");
    if (!f)
    {
        perror("Erreur, fopen dans prefixeMainPT");
        exit(1);
    }
    size_t fsize;
    char *s = readJSON(f, &fsize);
    PatriciaNode *pt = parseJSONPT(s, fsize);
    free(s);
    int nb_comp = 0;
    int nb_node = 0;
    printf("%d", nb_prefixe_patricia(pt, cle, &nb_node, &nb_comp));

    if (fclose(f) == EOF)
    {
        perror("Erreur, fclose dans prefixeMainPT");
        exit(1);
    }
    fprintf(stderr, "Nombre de comparaisons: %d\nNombre de noeuds visités:%d\n", nb_comp, nb_node);
    fprintf(stderr, "Hauteur de l'arbre: %d\n", hauteur_patricia(pt));
    free_patricia_node(pt);
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

    int node_count = 0, cmp_count = 0;
    printf("%d", prefixeTH(th, cle, &node_count, &cmp_count));
    fprintf(stderr, "[PREFIXE] Nombre de comparaisons de caractères: [%d]\n", cmp_count);
    fprintf(stderr, "[PREFIXE] Nombre de noeuds visités: [%d]\n", node_count);
    fprintf(stderr, "[PREFIXE] Hauteur: [%ld]\n", hauteurTH(th));
    fprintf(stderr, "[PREFIXE] Profondeur moyenne: [%f]\n", profondeurMoyenneTH(th));
    fprintf(stderr, "[PREFIXE] Longueur de la clé: [%lu]\n\n", lgueur(cle));

    if (fclose(f) == EOF)
    {
        perror("Erreur, fclose dans profondeurMoyenneMainTH");
        exit(1);
    }
    deleteTH(&th);
}
