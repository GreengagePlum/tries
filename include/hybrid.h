#ifndef HYBRID_H
#define HYBRID_H
/**
 * @file hybrid.h
 * @author Efe ERKEN (efe.erken@etu.sorbonne-universite.fr)
 * @brief Fichier d'entête contenant les déclaration des fonctions pour le Trie Hybride
 * @version 0.1
 * @date 2024-11-18
 *
 * @copyright Copyright (C) 2024 Efe ERKEN
 *
 */

#include <stddef.h>

/**
 * @def VALVIDE
 *
 * @brief La valeur d'un noeud pour indiquer qu'il ne constitue pas une fin de mot
 *
 */
#define VALVIDE 0

typedef struct trie_hybride
{
    char label; /**< Un caractère d'une clé stocké dans le trie */
    int value;  /**< Indicateur de fin de mot stocké si non nul */
    struct trie_hybride *inf, *eq, *sup;
} *TrieHybride;

/**
 * @brief Renvoie le premier caractère de la clé
 *
 * @param [in] cle Une chaine de caractères constituant une clé
 * @return Le premier caractère de la clé ou nul si la chaine est vide
 *
 * @pre La clé est terminé par un caractère nul
 * @pre La clé est composé des caractères ASCII (128 possibilités) encodé sur 8 bits
 *
 */
char prem(const char *cle);

/**
 * @brief Renvoie la clé privée de son premier caractère
 *
 * @param [in] cle Une chaine de caractères constituant une clé
 * @return Un pointeur sur les caractères de la clé suivant le premier ou nul si la chaine est vide
 *
 * @pre La clé est terminé par un caractère nul
 * @pre La clé est composé des caractères ASCII (128 possibilités) encodé sur 8 bits
 *
 */
const char *reste(const char *cle);

/**
 * @brief Renvoie le i−ème caractère de la clé
 *
 * @param [in] cle Une chaine de caractères constituant une clé
 * @param [in] i L'index d'un caractère de la clé commançant par 1
 * @return Le caractère demandé de la clé
 *
 * @pre L'index donné ne déborde pas la taille de la clé
 * @pre La clé est terminé par un caractère nul
 * @pre La clé est composé des caractères ASCII (128 possibilités) encodé sur 8 bits
 *
 */
char car(const char *cle, size_t i);

/**
 * @brief Renvoie le nombre de caractères de la clé
 *
 * @param [in] cle Une chaine de caractères constituant une clé
 * @return La taille en nombre de caractères de la clé >= 0
 *
 * @pre La clé est terminé par un caractère nul
 * @pre La clé est composé des caractères ASCII (128 possibilités) encodé sur 8 bits
 *
 */
size_t lgueur(const char *cle);

/**
 * @brief Crée un nouveau Trie Hybride vide
 *
 * @return Un pointeur sur le Trie Hybride créé
 *
 */
TrieHybride newTH(void);

/**
 * @brief Ajoute une clé dans le Trie Hybride donné
 *
 * @param [in,out] th Un pointeur vers le Trie Hybride à insérer la clé
 * @param [in] cle Une chaine de caractères constituant une clé
 * @param [in] v Une valeur non nul pour indiquer la fin du mot, peut être le numéro d'insértion
 * @return Un pointeur vers le Trie Hybride avec la clé ajouté
 *
 * @pre La clé est terminé par un caractère nul
 * @pre La clé est composé des caractères ASCII (128 possibilités) encodé sur 8 bits
 *
 */
TrieHybride ajoutTH(TrieHybride th, const char *restrict cle, int v);

/**
 * @brief Supprime une clé du Trie Hybride donné
 *
 * @param [in,out] th Un pointeur vers le Trie Hybride à supprimer la clé
 * @param [in] cle Une chaine de caractères constituant une clé
 * @return Un pointeur vers le Trie Hybride avec la clé supprimé
 *
 * @pre La clé est terminé par un caractère nul
 * @pre La clé est composé des caractères ASCII (128 possibilités) encodé sur 8 bits
 *
 */
TrieHybride supprTH(TrieHybride th, const char *restrict cle);

/**
 * @brief Libère l'espace occupé par le Trie Hybride donné
 *
 * @param [in,out] th Un pointeur de pointeur vers le Trie Hybride à libérer. Mis à nul à la fin.
 *
 * Libère en utilisant un parcours en postfix.
 *
 */
void deleteTH(TrieHybride *th);

#endif
