#ifndef HYBRID_H
#define HYBRID_H
/**
 * @file hybrid.h
 * @author Efe ERKEN (efe.erken@etu.sorbonne-universite.fr)
 * @brief Fichier d'entête contenant les déclaration des fonctions pour le Trie Hybride
 * @version 0.2
 * @date 2024-11-18
 *
 * @copyright Copyright (C) 2024 Efe ERKEN
 *
 */

#include <stdbool.h>
#include <stddef.h>

/**
 * @def VALVIDE
 *
 * @brief La valeur d'un noeud pour indiquer qu'il ne constitue pas une fin de mot
 *
 */
#define VALVIDE 0

/**
 * @def VALFIN
 *
 * @brief La valeur d'un noeud pour indiquer qu'il constitue bien une fin de mot
 *
 */
#define VALFIN 1

typedef struct trie_hybride
{
    char label; /**< Un caractère d'une clé stocké dans le trie */
    int value;  /**< Indicateur de fin de mot si non nul */
    struct trie_hybride *inf, *eq, *sup;
} *TrieHybride;

/**
 * @brief Renvoie le premier caractère de la clé
 *
 * @param [in] cle Une chaine de caractères constituant une clé
 * @return Le premier caractère de la clé. Si la fin de la chaine est atteinte cette valeur peut être '\0'
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
 * @return Un pointeur sur les caractères suivant le premier de la clé,
 * pointe sur le caractère '\0' si la fin de la chaine est atteinte,
 * ou nul s'il ne reste plus de caractères dans la chaine
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
 * Libère en utilisant un parcours en postfix. Le trie donné en paramètre peut être nul ou vide ou déjà désalloué, ces
 * cas sont gérés.
 *
 */
void deleteTH(TrieHybride *th);

/**
 * @brief Recherche une clé dans le Trie Hybride donné
 *
 * @param [in] th Un pointeur vers le Trie Hybride recherché
 * @param [in] cle Une chaine de caractères constituant une clé
 * @return Un booléen indiquant si la clé a été trouvé dans le trie ou pas
 *
 * @pre La clé est terminé par un caractère nul
 * @pre La clé est composé des caractères ASCII (128 possibilités) encodé sur 8 bits
 *
 */
bool rechercheTH(TrieHybride th, const char *restrict cle);

/**
 * @brief Compte le nombre de mots dans le Trie Hybride donné
 *
 * @param [in] th Un pointeur vers le Trie Hybride à parcourir
 * @return Un entier non signé indiquant le nombre de clés/mots trouvés
 *
 */
size_t comptageMotsTH(TrieHybride th);

/**
 * @brief Liste les mots du trie dans l'ordre alphabétique
 *
 * @param [in] th Un pointeur vers le Trie Hybride à parcourir
 * @return Un tableau, de dernier élément nul, de chaines de caractères avec tous les mots du trie en ordre alphabétique
 *
 * @post Le tableau retourné doit être libéré avec la fonction `deleteListeMots()`
 *
 */
char **listeMotsTH(TrieHybride th);

/**
 * @brief Libère la liste des mots créé par `listeMots()`
 *
 * @param [in] tab Le tableau des chaines de caractères créé par `listeMots()`
 *
 */
void deleteListeMotsTH(char **tab);

/**
 * @brief Compte les pointeurs vers nul
 *
 * @param [in] th Un pointeur vers le Trie Hybride à parcourir
 * @return Un entier non négatif indiquant le nombre de pointeurs vers nul
 *
 */
int comptageNilTH(TrieHybride th);

/**
 * @brief Calcule la hauteur du Trie Hybride donné
 *
 * @param [in] th Un pointeur vers le Trie Hybride à parcourir
 * @return Un entier non négatif indiquant la hauteur de l'arbre engandré par le trie
 *
 */
size_t hauteurTH(TrieHybride th);

/**
 * @brief Calcule la profondeur moyenne du Trie Hybride donné
 *
 * @param [in] th Un pointeur vers le Trie Hybride à parcourir
 * @return Un entier indiquant la profondeur moyenne de l'arbre engandré par le trie
 *
 * Le retour est -1 quand le trie donné est vide. L'entier retourné est obtenu en ne gardant seulement la partie entière
 * de la division réel sans arrondir.
 *
 */
int profondeurMoyenneTH(TrieHybride th);

/**
 * @brief Compte le nombre de mots dont la clé donné est le prefixe dans le trie donné
 *
 * @param [in] th Un pointeur vers le Trie Hybride à parcourir
 * @param [in] cle Une chaine de caractères constituant une clé
 * @return Un entier indiquant le nombre de mots dont cle est le prefixe dans le trie
 *
 * @pre La clé est terminé par un caractère nul
 * @pre La clé est composé des caractères ASCII (128 possibilités) encodé sur 8 bits
 *
 */
int prefixeTH(TrieHybride th, const char *cle);

#endif
