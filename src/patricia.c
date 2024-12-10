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
    PatriciaNode* curr = patricia;
    const char* rest = word;

    while (*rest) {
        int index = (unsigned char)(*rest);  //indice, qui est le premier char du mot

        // Cas 1: il n'y a pas de prefixe qui commence par le char donc on peut inserer 
        if (curr->prefixes[index] == NULL) {
            size_t len = strlen(rest);
            char* temp = malloc(len + 2);  // +2 pour EOE_CHAR et '\0'
            strcpy(temp, rest);
            temp[len] = EOE_CHAR;
            temp[len + 1] = '\0';

            curr->prefixes[index] = temp; 
            return;
        }

        // Cas 2 Il existe un prefixe
        char* prefix = curr->prefixes[index];
        int prefix_len = plus_long_pref(prefix, rest);  

        // le prefixe ne va plus terminer ici donc on bouge l'EOE CHAR
        if (prefix[prefix_len] == EOE_CHAR) {
            if (curr->children[index] == NULL) {
                curr->children[index] = create_patricia_node();
            }
            curr->children[index]->prefixes[EOE_INDEX] = strdup("\x01");  
            prefix[prefix_len] = '\0';  
        }

        // Cas 2a: match complet
        if (prefix_len == (int)strlen(prefix)) {
            rest += prefix_len;  

            if (*rest) {
                if (curr->children[index] == NULL) {
                    curr->children[index] = create_patricia_node();
                }
                curr = curr->children[index];  
            } else {
                if (curr->children[index] == NULL) {
                    curr->children[index] = create_patricia_node();
                }
                if (curr->children[index]->prefixes[EOE_INDEX] == NULL) {
                    curr->children[index]->prefixes[EOE_INDEX] = strdup("\x01");
                }
                return;
            }
            continue;
        }

        // Cas 2b: match partiel, il faut diviser le noeud
        PatriciaNode* new_node = create_patricia_node();

        int split_index = (unsigned char)prefix[prefix_len];
        new_node->prefixes[split_index] = strdup(prefix + prefix_len);
        new_node->children[split_index] = curr->children[index];


        prefix[prefix_len] = '\0';
        curr->children[index] = new_node;

    
        size_t len = strlen(rest);
        char* temp = malloc(len - prefix_len + 2);
        strcpy(temp, rest + prefix_len);
        temp[len - prefix_len] = EOE_CHAR;
        temp[len - prefix_len + 1] = '\0';
        new_node->prefixes[(unsigned char)rest[prefix_len]] = temp;

        return;
    }

    // Cas 3: fin du mot
    if (curr->children[EOE_INDEX] == NULL) {
        curr->children[EOE_INDEX] = create_patricia_node();
    }
    if (curr->children[EOE_INDEX]->prefixes[EOE_INDEX] == NULL) {
        curr->children[EOE_INDEX]->prefixes[EOE_INDEX] = strdup("\x01");
    }
}



int plus_long_pref(const char *s1, const char *s2) {
    int i = 0;
    while (s1[i] && s2[i] && s1[i] == s2[i]) i++;
    return i;
}


/**
 * @brief Fonction qui recherche un mot dans le Patricia-Trie
 * 
 * @param node 
 * @param word 
 * @return int 
 */
int recherche_patricia(PatriciaNode* node, const char* word) {
    PatriciaNode *curr = node;
    const char *rest = word;

    while (*rest) {
        int index = (unsigned char)(*rest);

        // Cas 1: il n'y a pas de préfixe qui commence par le char
        if (curr->prefixes[index] == NULL) {
            return 0;
        }

        char *prefix = curr->prefixes[index];
        int prefix_len = plus_long_pref(prefix, rest);

        // verifier la presence d'EOE char
        int actual_prefix_len = strlen(prefix);
        int has_eoe_char = (prefix[actual_prefix_len - 1] == EOE_CHAR) ? 1 : 0;
        if (has_eoe_char) {
            actual_prefix_len -= 1; 
        }

        // Cas où le préfixe correspond partiellement
        if (prefix_len != actual_prefix_len) {
            return 0;
        }

        rest += prefix_len;


        if (*rest == '\0') {
            // verifier la presence d'EOE char dans l'enfant
            if (has_eoe_char || (curr->children[index]->prefixes[EOE_INDEX] != NULL)) {
                return 1; 
            }
            return 0; 
        }

    
        if (curr->children[index] == NULL) {
            return 0;
        }

        curr = curr->children[index];
    }

    return 0;
}


void remove_eoe_char(char* s) {
    size_t len = strlen(s);
    if (len > 0 && s[len - 1] == EOE_CHAR) {
        s[len - 1] = '\0';
    }
}

int estvide(PatriciaNode* node) {
    for (int i = 0; i < ASCII_SIZE; i++) {
        if (node->prefixes[i] != NULL || node->children[i] != NULL) {
            return 0;
        }
    }
    return 1;
}

char* string_concat(const char* s1, const char* s2) {
    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    char* res = malloc(len1 + len2 + 1);
    strcpy(res, s1);
    strcat(res, s2);
    return res;
}

void free_patricia_node(PatriciaNode* node) {
    if (node == NULL) {
        return;
    }
    for (int i = 0; i < ASCII_SIZE; i++) {
        if (node->prefixes[i] != NULL) {
            free(node->prefixes[i]);
        }
        if (node->children[i] != NULL) {
            free_patricia_node(node->children[i]);
        }
    }
    free(node);
}

int delete_word(PatriciaNode* node, const char* word){
    PatriciaNode* curr = node;
    const char* rest = word;
    rest += strlen(word);
    curr = curr->children[(unsigned char)*rest];
    return 1;
}


int comptage_mots_patricia(PatriciaNode* node) {
    int count = 0;
    for (int i = 0; i < ASCII_SIZE; i++) {
        if (node->prefixes[i] != NULL && has_eoe_char(node->prefixes[i])) {
            count++;
        }
        if (node->children[i] != NULL) {
            count += comptage_mots_patricia(node->children[i]);
        }
    }
    return count;
}

int has_eoe_char(const char* s) {
    size_t len = strlen(s);
    return (len > 0 && s[len - 1] == EOE_CHAR) ? 1 : 0;
}

int comptage_nil_patricia(PatriciaNode* node) {
    int count = 0;
    for (int i = 0; i < ASCII_SIZE; i++) {
        if (node->prefixes[i] == NULL) {
            count++;
        }
        if (node->children[i] != NULL) {
            count += comptage_nil_patricia(node->children[i]);
        }
    }
    return count;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

int hauteur_patricia(PatriciaNode* node) {
    int height = 0;
    for (int i = 0; i < ASCII_SIZE; i++) {
        if (node->children[i] != NULL) {
            height = max(height, hauteur_patricia(node->children[i]));
        }
    }
    return height + 1;
}

int nb_prefixe_patricia(PatriciaNode* node, const char* word) {
    if (node == NULL || *word == '\0') {
        return 0;
    }

    int index = (unsigned char)(*word);

    if (node->prefixes[index] == NULL) {
    return 0;
    }
    int prefix = plus_long_pref(node->prefixes[index], word); 

    if(est_prefixe(word, node->prefixes[index])){
        if(has_eoe_char(node->prefixes[index])){
            return 1;
        }
        else{
            return comptage_mots_patricia(node->children[index]);
        }
    }
    if(prefix < (int)strlen(word) && prefix < (int)strlen(node->prefixes[index])){
        return 0;
    }

    if(prefix == (int)strlen(node->prefixes[index])){
        if(node->children[index] == NULL){
            return 0;
        }
        return nb_prefixe_patricia(node->children[index], word + prefix);
    }

    return 0;
}

int est_prefixe(const char* s1, const char* s2){
    int i = 0;
    while(s1[i] && s2[i] && s1[i] == s2[i]){
        i++;
    }
    return s1[i] == '\0';
}



char** liste_mots_patricia(PatriciaNode* node) {
    static char* res[MAX_WORDS]; 
    static int index = 0;  
    index = 0; 

    liste_mots_patricia_recursive(node, "", res, &index);

    return res;
}

void liste_mots_patricia_recursive(PatriciaNode* node, const char* prefix, char** res, int* index) {
    for (int i = 0; i < ASCII_SIZE; i++) {
        if (node->prefixes[i] != NULL) {
        
            char new_prefix[MAX_WORD_LENGTH];
            snprintf(new_prefix, MAX_WORD_LENGTH, "%s%s", prefix, node->prefixes[i]);

        
            if (has_eoe_char(node->prefixes[i])) {
                res[*index] = strdup(new_prefix); 
                (*index)++;
            }

            if (node->children[i] != NULL) {
    
                liste_mots_patricia_recursive(node->children[i], new_prefix, res, index);
            }
        }
    }
}

void print_list_patricia(char** list, int size) {
    printf("[ ");
    for (int i = 0; i < size-1; i++) {
        printf("%s, ", list[i]);
    }
    printf("%s ]\n", list[size-1]);

}

int profondeur_moyenne_patricia_feuille(PatriciaNode* node){
    int sum = 0;
    int nbFeuilles = 0;

    calcule_profondeur_moyenne_patricia_feuille(node, 0, &sum, &nbFeuilles);
    if(nbFeuilles == 0){
        return 0;
    }
    else {
        return sum / nbFeuilles;
    }
}

void calcule_profondeur_moyenne_patricia_feuille(PatriciaNode* node, int profondeur, int* sum, int* nbFeuilles){
    if(estvide(node)){
        return;
    }

    for(int i = 0; i < ASCII_SIZE; i++){
        if(node->prefixes[i] != NULL){
            if(has_eoe_char(node->prefixes[i])){
                *sum += profondeur + 1;
                (*nbFeuilles)++;
            }
            else{
                calcule_profondeur_moyenne_patricia_feuille(node->children[i], profondeur + 1, sum, nbFeuilles);
            }
        }
    }
}