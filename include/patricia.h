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
#define MAX_WORDS 1000 
#define MAX_WORD_LENGTH 100  

typedef struct PatriciaNode{
    char *prefixes[ASCII_SIZE];
    struct PatriciaNode *children[ASCII_SIZE];
} PatriciaNode;

PatriciaNode *create_patricia_node();
void insert_patricia(PatriciaNode *root, const char *mot);
int plus_long_pref(const char *s1, const char *s2);
int recherche_patricia(PatriciaNode* patricia, const char* word);
void free_patricia_node(PatriciaNode* node);
void remove_eoe_char(char* s);
int delete_word(PatriciaNode* node, const char* word);
int comptage_mots_patricia(PatriciaNode* node);
int has_eoe_char(const char* s);
int comptage_nil_patricia(PatriciaNode* node);
int hauteur_patricia(PatriciaNode* node);
int nb_prefixe_patricia(PatriciaNode* node, const char* word);
int est_prefixe(const char* s1, const char* s2);
int max(int a, int b);
char** liste_mots_patricia(PatriciaNode* node);
void liste_mots_patricia_recursive(PatriciaNode* node, const char* prefix, char** res, int* index);
void print_list_patricia(char** liste, int size);

#endif
