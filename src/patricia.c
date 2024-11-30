/**
 * @file patricia.c
 * @author Efe ERKEN (efe.erken@etu.sorbonne-universite.fr)
 * @brief Fichier source contenant les corps des fonctions pour le Patricia-Trie
 * @version 0.1
 * @date 2024-11-18
 *
 * @copyright Copyright (C) 2024 Efe ERKEN
 *
 */
#define _DEFAULT_SOURCE 1
#include "patricia.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>



/**
 * @brief Fonction qui crée un nouveau noeud Patricia
 * 
 * @return PatriciaNode* 
 */
PatriciaNode *create_patricia_node(){
    PatriciaNode *node = (PatriciaNode *)malloc(sizeof(PatriciaNode));
    for(int i = 0; i < ASCII_SIZE; i++){
        node->prefixes[i] = NULL;
        node->children[i] = NULL;
    }
    return node;
}

/**
 * @brief Fonction qui insère un mot dans le Patricia-Trie
 * 
 * @param root 
 * @param word 
 */
void insert_patricia(PatriciaNode* patricia, const char* word) {
    PatriciaNode *curr = patricia;
    const char *rest = word;

    while (*rest) {
        int index = (unsigned char)(*rest);

        // Cas 1: Pas de prefixe, on ajoute le mot ici
        if (curr->prefixes[index] == NULL) {

            size_t len = strlen(rest);
            char *temp = malloc(len + 2); // +2 pour EOE_CHAR and '\0'
            strcpy(temp, rest);
            temp[len] = EOE_CHAR; 
            temp[len + 1] = '\0'; 

            curr->prefixes[index] = temp;  
            return;
        }

        // Cas 2: Il y a un prefixe
        char *prefix = curr->prefixes[index];
        int prefix_len = plus_long_pref(prefix, rest);

        // Cas 2a: Prefixe complet, on continue
        if (prefix_len == (int)strlen(prefix)) {
            rest += prefix_len;
            
            if (*rest) {
                // Si c'est pas la fin du mot on continue
                if (curr->children[index] == NULL) {
                    curr->children[index] = create_patricia_node();
                }
                curr = curr->children[index];
            } else {
                if (curr->prefixes[EOE_CHAR] == NULL) {
                    curr->prefixes[EOE_CHAR] = strdup("\x01");
                }
            }
            continue;
        }

        // Cas 2b: Prefixe partiel
        PatriciaNode *new_node = create_patricia_node();
        
        // le noeud courant devient le prefixe en commun
        new_node->prefixes[(unsigned char)prefix[prefix_len]] = strdup(prefix + prefix_len);
        new_node->children[(unsigned char)prefix[prefix_len]] = curr->children[index];

        curr->prefixes[index][prefix_len] = '\0';
        curr->children[index] = new_node;

        // le nouveau noeud sera le reste du mot
        size_t len = strlen(rest);
        char *temp = malloc(len - prefix_len + 2); // +2 pour EOE_CHAR and '\0'
        strcpy(temp, rest + prefix_len);
        temp[len - prefix_len] = EOE_CHAR;
        temp[len - prefix_len + 1] = '\0';
        new_node->prefixes[(unsigned char)rest[prefix_len]] = temp;
        return;
    }

    // Cas 3: Fin du mot
    if (curr->prefixes[EOE_CHAR] == NULL) {
        curr->prefixes[EOE_CHAR] = strdup("\x01");
    }
}

int plus_long_pref(const char *s1, const char *s2) {
    int i = 0;
    while (s1[i] && s2[i] && s1[i] == s2[i]) i++;
    return i;
}






