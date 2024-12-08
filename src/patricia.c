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
#include <stdbool.h>



/**
 * @brief Fonction qui crée un nouveau noeud Patricia
 * 
 * @return PatriciaNode* 
 */
PatriciaNode *create_patricia_node(){
    PatriciaNode *node = (PatriciaNode *)malloc(sizeof(PatriciaNode));
    for(int i = 0; i < ASCII_SIZE; i++){
        node->children[i] = NULL;
    }
    node->label = NULL;
    return node;
}

void free_patricia(PatriciaNode *node){
    if(!node){
        return;
    }
    for(int i = 0; i < ASCII_SIZE; i++){
        if(node->children[i]){
            free_patricia(node->children[i]);
        }
    }
    if(node->label){
        free(node->label);
    }
    free(node);
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
        int index = (unsigned char)(*rest); 

        // cas 1 : word n'est pas prefixe dans l'arbre
        if (curr->children[index] == NULL) {
            size_t len = strlen(rest);
            char* temp = (char*)malloc(len + 2); 
            strcpy(temp, rest);
            temp[len] = EOE_CHAR;
            temp[len + 1] = '\0';

            curr->children[index] = create_patricia_node();
            curr->children[index]->label = temp;
            return;
        }

        // Cas 2 : il y a un prefixe
        PatriciaNode* child = curr->children[index];
        int prefix_len = plus_long_pref(child->label, rest);


        if (prefix_len == (int)strlen(child->label)) {
            rest += prefix_len;
            curr = child;
            continue;
        }

        
        PatriciaNode* new_node = create_patricia_node();
        new_node->label = strdup(child->label + prefix_len);

        
        char* old_label = child->label;
        child->label = (char*)malloc(prefix_len + 1);
        strncpy(child->label, old_label, prefix_len);
        child->label[prefix_len] = '\0';


        for (int i = 0; i < ASCII_SIZE; i++) {
            if (child->children[i] != NULL) {  
                new_node->children[i] = child->children[i];
                child->children[i] = NULL;
            }
        }

    
        int child_index = (unsigned char)new_node->label[0];
        child->children[child_index] = new_node;

    
        if (has_eoe_char(old_label)) {
            child->children[EOE_INDEX] = create_patricia_node();
            child->children[EOE_INDEX]->label = strdup("\x01");
        }

        free(old_label);

        // Insert the rest of the word
        curr->children[index] = child;
        curr = child;

        size_t len = strlen(rest);
        char* temp = (char*)malloc(len - prefix_len + 2);  // +2 for EOE_CHAR and '\0'
        strcpy(temp, rest + prefix_len);
        temp[len - prefix_len] = EOE_CHAR;
        temp[len - prefix_len + 1] = '\0';

        curr->children[(unsigned char)rest[prefix_len]] = create_patricia_node();
        curr->children[(unsigned char)rest[prefix_len]]->label = temp;

        return;
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
 * @return bool
  */
bool recherche_patricia(PatriciaNode* node, const char* word) {
    PatriciaNode* curr = node;
    const char* rest = word;

    while (*rest) {
        int index = (unsigned char)(*rest);
        if (curr->children[index] == NULL) {
            return false;
        }

        PatriciaNode* child = curr->children[index];
        int prefix_len = plus_long_pref(child->label, rest);
        int label_len = (int)strlen(child->label);
    
        if(has_eoe_char(child->label) && prefix_len == (label_len-1)){
            if(prefix_len == (int)strlen(rest)){
                return true;
            }
            return false;
        }
        
        if(prefix_len < (int)strlen(child->label)){
            return false;
        }

        rest += prefix_len;
        if(*rest == '\0'){
            if(has_eoe_char(child->label)){
                return true;
            }
            if(child->children[EOE_INDEX] != NULL){
                return true;
            }
            return false;
        }
        curr = child;
    }

    return false;
}

bool has_eoe_char(const char* s) {
    size_t len = strlen(s);
    return (len > 0 && s[len - 1] == EOE_CHAR) ? true : false;
}

int comptage_mots_patricia(PatriciaNode* node) {
    if (!node) {
        return 0;
    }

    int cpt = 0;

    if (node->label != NULL && has_eoe_char(node->label)) {
        cpt++; 
    } else {
        for (int i = 1; i < ASCII_SIZE; i++) {
            if (node->children[i]) {
                cpt += comptage_mots_patricia(node->children[i]);
            }
        }
    }

    return cpt;
}

int comptage_nul_patricia(PatriciaNode* node) {
    if (!node) {
        return 0;
    }

    int cpt = 0;

    if (node->label == NULL) {
        cpt++;
    } else {
        for (int i = 1; i < ASCII_SIZE; i++) {
            if (node->children[i]) {
                cpt += comptage_nul_patricia(node->children[i]);
            }
        }
    }

    return cpt;
}

int hauteur_patricia(PatriciaNode* node){
    if(!node){
        return 0;
    }

    int max_height = 0;
    for(int i = 1; i < ASCII_SIZE; i++){
        if(node->children[i]){
            int height = hauteur_patricia(node->children[i]);
            if(height > max_height){
                max_height = height;
            }
        }
    }

    return 1 + max_height;
}

int profondeur_moyenne(PatriciaNode* node){
    if(!node){
        return 0;
    }

    int sum = 0;
    int count = 0;
    
    profondeur_rec(node, 0, &sum, &count);

    return (count > 0) ? sum / count : 0;
}

void profondeur_rec(PatriciaNode* node, int profondeur, int* sum, int* count){
    if(!node){
        return;
    }
    *sum += profondeur;
    *count += 1;

    for(int i = 1; i < ASCII_SIZE; i++){
        if(node->children[i]){
            profondeur_rec(node->children[i], profondeur + 1, sum, count);
        }
    }
}