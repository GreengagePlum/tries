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
#include "cJSON.h"
#include "patricia.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>


/**
 * @brief Fonction qui crée un nouveau noeud Patricia
 *
 * @return PatriciaNode*
 */
PatriciaNode *create_patricia_node(void){
    PatriciaNode *node = (PatriciaNode *)malloc(sizeof(PatriciaNode));
    if(node == NULL){
        fprintf(stderr, "Erreur d'allocation de mémoire: create_patricia_node\n");
        exit(1);
    }
    for(int i = 0; i < ASCII_SIZE; i++){
        node->prefixes[i] = NULL;
        node->children[i] = NULL;
    }
    return node;
}

const char* suffixe(const char* s1, const char* s2){
    if(s1 == NULL || s2 == NULL){
        return NULL;
    }
    if(est_prefixe(s1, s2)){
        return s2 += plus_long_pref(s1,s2);
    }
    else{
        return NULL;
    }
}

/**
 * @brief Fonction qui insère un mot dans le Patricia-Trie
 *
 * @param root
 * @param word
 */
void insert_patricia(PatriciaNode* patricia, const char* word) {
    if (*word == '\0') {
        if (patricia->prefixes[EOE_INDEX] == NULL) {
            patricia->prefixes[EOE_INDEX] = strdup(" ");
        }
        return;
    }

    int index = (unsigned char)(*word);

    if (patricia->prefixes[index] == NULL) {
        patricia->prefixes[index] = strdup(word);
        patricia->children[index] = NULL;
        return;
    }

    if (est_prefixe(patricia->prefixes[index], word)) {
        if (patricia->children[index] == NULL) {
            patricia->children[index] = create_patricia_node();
            patricia->children[index]->prefixes[EOE_INDEX] = strdup(" ");
        }
        insert_patricia(patricia->children[index], suffixe(patricia->prefixes[index], word));
        return;
    }

    int label_len = (int)strlen(patricia->prefixes[index]);
    int prefix_commun = plus_long_pref(word, patricia->prefixes[index]);

    if (prefix_commun < label_len) {
        char* copy = strndup(patricia->prefixes[index], prefix_commun);
        char* copy2 = strdup(patricia->prefixes[index] + prefix_commun);


        PatriciaNode* new_child = create_patricia_node();

        new_child->prefixes[(unsigned char)(*copy2)] = copy2;
        new_child->children[(unsigned char)(*copy2)] = patricia->children[index];
        patricia->children[index] = new_child;



        free(patricia->prefixes[index]);
        patricia->prefixes[index] = copy;

        insert_patricia(patricia->children[index], word + prefix_commun);
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
    if (node == NULL) {
        return 0;
    }

    if (*word == '\0') {
        return node->prefixes[EOE_INDEX] != NULL;
    }

    int index = (unsigned char)(*word);

    if (node->prefixes[index] == NULL) {
        return 0;
    }

    int prefix_len = (int)strlen(node->prefixes[index]);
    int common_prefix_len = plus_long_pref(node->prefixes[index], word);

    if (common_prefix_len == prefix_len) {
        if ((int)strlen(word) == prefix_len) {
            return node->prefixes[EOE_INDEX] != NULL || node->children[index] == NULL;
        }
        return recherche_patricia(node->children[index], word + common_prefix_len);
    }


    return 0;
}



char* string_concat(const char* s1, const char* s2) {
    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    char* res = malloc(len1 + len2 + 1);
    if (res == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire: string_concat\n");
        exit(1);
    }
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
    if (node == NULL) {
        return 0;
    }

    if(*word == '\0'){
        if(node->prefixes[EOE_INDEX] != NULL){
            free(node->prefixes[EOE_INDEX]);
            node->prefixes[EOE_INDEX] = NULL;
            return 1;
        }
        return 0;
    }

    int index = (unsigned char)(*word);
    //printf("Comparing %s and %s\n Result: %d\n",word, node->prefixes[index], strcmp(word, node->prefixes[index]));//debug
    if(node->prefixes[index] == NULL){
        return 0;
    }
    if((strcmp(word, node->prefixes[index]) == 0) && node->children[index] == NULL){
        free(node->prefixes[index]);
        node->prefixes[index] = NULL;
        return 1;
    }

    else{
        const char* s = suffixe(node->prefixes[index], word);
        if(s == NULL){
            return 0;
        }
        if(node->children[index] == NULL){
            return 0;
        }
        int i = delete_word(node->children[index], s);
        if(i == 0){
            return 0;
        }
        int nb_enfant = 0;
        int index_enfant = 0;
        for(int i = 0; i < ASCII_SIZE; i++){
            if(node->children[index]->prefixes[i] != NULL){
                nb_enfant++;
                index_enfant = i;
            }
        }

        if(nb_enfant == 1){
            if(index_enfant == EOE_INDEX){
                free_patricia_node(node->children[index]);
                node->children[index] = NULL;
            }
            else{
                char* new_prefix = string_concat(node->prefixes[index], node->children[index]->prefixes[index_enfant]);
                free(node->prefixes[index]);
                free(node->children[index]->prefixes[index_enfant]);
                node->prefixes[index] = new_prefix;
                PatriciaNode* new_child = node->children[index]->children[index_enfant];
                free(node->children[index]);
                node->children[index] = new_child;
            }
        }
        return 1;
    }
    return 0;

}


int comptage_mots_patricia(PatriciaNode* node) {
    int count = 0;
    for (int i = 0; i < ASCII_SIZE; i++) {
        if(i == EOE_INDEX){
            if(node->prefixes[i] != NULL){
                count++;
            }
        }
        else if (node->prefixes[i] != NULL && node->children[i] == NULL) {
            count++;
        }
        if (node->children[i] != NULL) {
            count += comptage_mots_patricia(node->children[i]);
        }
    }
    return count;
}

int has_eoe_char(const char* s) {
    return s != NULL && s[strlen(s) - 1] == EOE_CHAR;
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
        if(node->children[index] == NULL){
            return 1;
        }
        else{
            return comptage_mots_patricia(node->children[index]);
        }
    }
    if(prefix == (int)strlen(node->prefixes[index])){
        if(node->children[index] == NULL){
            return 1;
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

            if(i != EOE_INDEX){
                snprintf(new_prefix, MAX_WORD_LENGTH, "%s%s", prefix, node->prefixes[i]);
            }
            else{
                snprintf(new_prefix, MAX_WORD_LENGTH, "%s%c", prefix, '\0');
            }

            if (node->children[i] == NULL || node->prefixes[EOE_CHAR] != NULL) {
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

void free_list_patricia(char** list, int size) {
    for (int i = 0; i < size; i++) {
        free(list[i]);
    }
}

void print_patricia(PatriciaNode* node, int depth) {
    if (node == NULL) {
        return;
    }

    for (int i = 0; i < ASCII_SIZE; i++) {
        if (node->prefixes[i] != NULL) {
            for (int j = 0; j < depth; j++) {
                printf("  ");
            }
            if (i == EOE_INDEX) {
                printf("%c: %s\n", EOE_CHAR, node->prefixes[i]);
            } else {
            printf("%c: %s\n", i, node->prefixes[i]);}
            print_patricia(node->children[i], depth + 1);
        }
    }
}
float profondeur_moyenne_patricia_feuille(PatriciaNode* node){
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
    if(node == NULL){
        return;
    }

    for(int i = 0; i < ASCII_SIZE; i++){
        if(node->prefixes[i] != NULL){
            if(node->children[i] == NULL){
                *sum += profondeur + 1;
                (*nbFeuilles)++;
            }
            else{
                calcule_profondeur_moyenne_patricia_feuille(node->children[i], profondeur + 1, sum, nbFeuilles);
            }
        }
    }
}

void prefixe_fusion(const char* s1, const char* s2, char* x, char* y, char* z){
    int prefix_len = plus_long_pref(s1, s2);
    strncpy(x, s1, prefix_len);
    x[prefix_len] = '\0';
    strcpy(y, s1 + prefix_len);
    strcpy(z, s2 + prefix_len);
}

PatriciaNode* pat_cons(PatriciaNode* node, const char* word){
    PatriciaNode* new_node = create_patricia_node();
    insert_patricia(new_node, word);
    if(node == NULL){
        return new_node;
    }
    int index = (unsigned char)(*word);
    new_node->children[index] = node;

    return new_node;
}

PatriciaNode* pat_fusion(PatriciaNode* node1, PatriciaNode* node2){
    if(node1 == NULL){
        return node2;
    }
    if(node2 == NULL){
        return node1;
    }

    for(int i = 0; i < ASCII_SIZE; i++){
        if(node1->prefixes[i] == NULL && node2->prefixes[i] != NULL){

           node1->prefixes[i] = strdup(node2->prefixes[i]);
           free(node2->prefixes[i]);
           node2->prefixes[i] = NULL;
           node1->children[i] = node2->children[i];
           node2->children[i] = NULL;
        }
        else if(node1->prefixes[i] != NULL && node2->prefixes[i] != NULL){
           if(strcmp(node1->prefixes[i], node2->prefixes[i]) == 0){
                if(i == EOE_INDEX){
                    
                }
                else if (node1->children[i] == NULL){
                    if(node2->children[i] != NULL){
                        node1->children[i] = create_patricia_node();
                        node1->children[i]->prefixes[EOE_INDEX] = strdup(" ");
                    }
                }
                else if(node2->children[i] == NULL){
                    if(node1->children[i]->prefixes[EOE_INDEX] == NULL){
                        node1->children[i]->prefixes[EOE_INDEX] = strdup(" ");
                    }
                }

                free(node2->prefixes[i]);
                node1->children[i] = pat_fusion(node1->children[i], node2->children[i]);
                node2->children[i] = NULL;
                //free_patricia_node(node2->children[i]);
           }
           else{
                const char* suf = suffixe(node1->prefixes[i], node2->prefixes[i]);
                const char* suf2 = suffixe(node2->prefixes[i], node1->prefixes[i]);
                if(suf){
                    PatriciaNode* new_node = pat_cons(node2->children[i], suf);
                    if(node1->children[i] == NULL){
                        node1->children[i] = create_patricia_node();
                        node1->children[i]->prefixes[EOE_INDEX] = strdup(" ");
                    }
                    free(node2->prefixes[i]);
                    node1->children[i] = pat_fusion(node1->children[i], new_node);
                    node2->children[i] = NULL;

                }
                else if(suf2){
                    PatriciaNode* new_node = pat_cons(node1->children[i], suf2);
                    if(node2->children[i] == NULL ){
                        node2->children[i] = create_patricia_node();
                        node2->children[i]->prefixes[EOE_INDEX] = strdup(" ");
                    }
                    free(node1->prefixes[i]);
                    node1->prefixes[i] = strdup(node2->prefixes[i]);
                    free(node2->prefixes[i]);
                    node1->children[i] = pat_fusion(new_node, node2->children[i]);
                    node2->children[i] = NULL;
                }
                else{
                    char* x = malloc(sizeof(char) * MAX_WORD_LENGTH);
                    char* y = malloc(sizeof(char) * MAX_WORD_LENGTH);
                    char* z = malloc(sizeof(char) * MAX_WORD_LENGTH);
                    prefixe_fusion(node1->prefixes[i], node2->prefixes[i], x, y, z);
                    PatriciaNode* PT1 = pat_cons(node1->children[i], y);
                    PatriciaNode* PT2 = pat_cons(node2->children[i], z);
                    PatriciaNode* A_prim= pat_fusion(PT1, PT2);
                    free(node1->prefixes[i]);
                    node1->prefixes[i] = strdup(x);
                    node1->children[i] = A_prim;
                    free(node2->prefixes[i]);
                    node2->prefixes[i] = NULL;
                    node2->children[i] = NULL;
                    free(x);
                    free(y);
                    free(z);
                }
           }
        }
        else{

           }
    }
    free(node2);
    return node1;
}

cJSON *constructJSONPT(const PatriciaNode *node, const char *label)
{
    cJSON *obj = cJSON_CreateObject();
    if (!obj)
    {
        fprintf(stderr, "Erreur, cJSON_CreateObject dans constructJSONPT");
        exit(1);
    }
    if (!cJSON_AddStringToObject(obj, "label", label))
    {
        fprintf(stderr, "Erreur, cJSON_AddStringToObject dans constructJSONPT");
        exit(1);
    }
    bool isEnd = node ? !!node->prefixes[EOE_INDEX] : true;
    if (!cJSON_AddBoolToObject(obj, "is_end_of_word", isEnd))
    {
        fprintf(stderr, "Erreur, cJSON_AddBoolToObject dans constructJSONPT");
        exit(1);
    }
    cJSON *children = cJSON_CreateObject();
    if (!children)
    {
        fprintf(stderr, "Erreur, cJSON_CreateObject dans constructJSONPT");
        exit(1);
    }
    if (node)
    {
        for (int i = 1; i < ASCII_SIZE; i++)
        {
            if (node->prefixes[i])
            {
                cJSON *offspring = constructJSONPT(node->children[i], node->prefixes[i]);
                char buf[2] = {(char)i, 0};
                if (!cJSON_AddItemToObject(children, buf, offspring))
                {
                    fprintf(stderr, "Erreur, cJSON_AddItemToObject dans constructJSONPT");
                    exit(1);
                }
            }
        }
    }
    if (!cJSON_AddItemToObject(obj, "children", children))
    {
        fprintf(stderr, "Erreur, cJSON_AddItemToObject dans constructJSONPT");
        exit(1);
    }
    return obj;
}

char *printJSONPT(const PatriciaNode *node)
{
    assert(node && "Given Patricia-Trie can't be null here");
    cJSON *json = constructJSONPT(node, "");
    char *res = cJSON_Print(json);
    cJSON_Delete(json);
    return res;
}

bool parseJSONPT_hasChildren(const cJSON *json)
{
    int count = 0;
    const cJSON *elem;
    cJSON_ArrayForEach(elem, json)
    {
        count++;
    }
    return !!count;
}

PatriciaNode *parseJSONPT_rec(const cJSON *json, char **label)
{
    const cJSON *obj;

    obj = cJSON_GetObjectItemCaseSensitive(json, "label");
    assert(cJSON_IsString(obj) && "'label' element has to be a JSON string element");
    *label = strdup(obj->valuestring);
    if (!*label)
    {
        fprintf(stderr, "Erreur, strdup dans parseJSONPT_rec");
        exit(1);
    }

    const cJSON *children;
    children = cJSON_GetObjectItemCaseSensitive(json, "children");
    assert(cJSON_IsObject(children) && "'children' element has to be a JSON object element");
    bool hasChildren = parseJSONPT_hasChildren(children);
    if (!hasChildren)
        return NULL;

    obj = cJSON_GetObjectItemCaseSensitive(json, "is_end_of_word");
    assert(cJSON_IsBool(obj) && "'is_end_of_word' element has to be a JSON bool element");
    bool isEnd = cJSON_IsTrue(obj);

    PatriciaNode *node = create_patricia_node();
    char *rec_label;
    const cJSON *elem;
    cJSON_ArrayForEach(elem, children)
    {
        unsigned char index = elem->string[0];
        node->children[index] = parseJSONPT_rec(elem, &rec_label);
        node->prefixes[index] = rec_label;
    }

    if (hasChildren)
    {
        if (isEnd)
        {
            char *end_str = strdup(" ");
            if (!end_str)
            {
                fprintf(stderr, "Erreur, strdup dans parseJSONPT_rec");
                exit(1);
            }
            node->prefixes[EOE_INDEX] = end_str;
        }
        else
        {
            node->prefixes[EOE_INDEX] = NULL;
        }
    }

    return node;
}

PatriciaNode *parseJSONPT(const char *json, size_t sz)
{
    cJSON *obj = cJSON_ParseWithLength(json, sz);
    if (!obj)
    {
        fprintf(stderr, "Erreur, cJSON_ParseWithLength dans parseJSONPT");
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        exit(1);
    }
    char *label;
    PatriciaNode *th = parseJSONPT_rec(obj, &label);
    free(label);
    cJSON_Delete(obj);
    return th;
}
