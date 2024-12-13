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
#define MAX_WORDS 10000 
#define MAX_WORD_LENGTH 100  
#include "cJSON.h"
#include <stdbool.h>

typedef struct PatriciaNode{
    char *prefixes[ASCII_SIZE];
    struct PatriciaNode *children[ASCII_SIZE];
} PatriciaNode;

//FONCTION AUXILIAIRES

const char* suffixe(const char* s1, const char* s2);
int plus_long_pref(const char *s1, const char *s2);
int est_prefixe(const char* s1, const char* s2);
void free_patricia_node(PatriciaNode* node);
void remove_eoe_char(char* s);
int has_eoe_char(const char* s);
int max(int a, int b);
void print_list_patricia(char** liste, int size);
void print_patricia(PatriciaNode* node, int depth);
void prefixe_fusion(const char* s1, const char* s2, char* x, char* y, char* z);
PatriciaNode* pat_cons(PatriciaNode* node, const char* word);
void free_list_patricia(char** list, int size);

//FONCTIONS PRINCIPALES
PatriciaNode *create_patricia_node(void);

void insert_patricia(PatriciaNode *root, const char *mot);

int recherche_patricia(PatriciaNode* patricia, const char* word);

int nb_prefixe_patricia(PatriciaNode* node, const char* word);

int delete_word(PatriciaNode* node, const char* word);

int comptage_mots_patricia(PatriciaNode* node);

int comptage_nil_patricia(PatriciaNode* node);

int hauteur_patricia(PatriciaNode* node);

char** liste_mots_patricia(PatriciaNode* node);

int profondeur_moyenne_patricia_feuille(PatriciaNode* node);

PatriciaNode* pat_fusion(PatriciaNode* node1, PatriciaNode* node2);

//HELPERS RECURSIVES
void calcule_profondeur_moyenne_patricia_feuille(PatriciaNode* node, int profondeur, int* sum, int* nbFeuilles);

void liste_mots_patricia_recursive(PatriciaNode* node, const char* prefix, char** res, int* index);

//CJSON

cJSON *constructJSONPT(const PatriciaNode *node);
char *printJSONPatricia(const PatriciaNode *node);
cJSON *constructJSONPT_helper(const PatriciaNode *node, const char *prefix);
PatriciaNode *parseJSONPT(const char *json, size_t sz);
#endif
