#ifndef PATRICIA_H
#define PATRICIA_H
/**
 * @file patricia.h
 * @author Efe ERKEN (efe.erken@etu.sorbonne-universite.fr)
 * @brief Fichier d'entête contenant les déclaration des fonctions pour le Patricia-Trie
 * @version 0.1
 * @date 2024-11-18
 *
 * @copyright Copyright (C) 2024 Efe ERKEN
 *
 */
#define ASCII_SIZE 128
#define EOE_INDEX 0
#define EOE_CHAR '\x01'

typedef struct PatriciaNode{
    char *prefixes[ASCII_SIZE];
    struct PatriciaNode *children[ASCII_SIZE];
} PatriciaNode;

PatriciaNode *create_patricia_node();
void insert_patricia(PatriciaNode *root, const char *mot);
int plus_long_pref(const char *s1, const char *s2);

#endif
