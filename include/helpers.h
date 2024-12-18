#ifndef HELPERS_H
#define HELPERS_H
/**
 * @file helpers.h
 * @author Efe ERKEN (efe.erken@etu.sorbonne-universite.fr)
 * @brief Fichier d'entête contenant les déclaration des fonctions pour gérer l'interface utilisateur du programme
 * @version 0.2
 * @date 2024-12-05
 *
 * @copyright Copyright (C) 2024 Efe ERKEN
 *
 */

#include <stdbool.h>

/**
 * @brief Lit une liste de mots depuis l'entrée standard et écrit le Patricia-Trie correspondant sous format JSON en
 * sortie standard
 *
 * @pre Les mots sont composés des caractères ASCII (128 possibilités) encodé sur 8 bits
 *
 * La liste de mots donné dans l'entrée standard doit être sous forme d'un mot par ligne. La sortie standard contiendra
 * le Patricia-Trie correspondant (construit par ajouts successifs des mots donnés) sous format JSON.
 *
 */
void insererPT(void);

/**
 * @brief Lit une liste de mots depuis l'entrée standard et écrit le Trie Hybride correspondant sous format JSON en
 * sortie standard
 *
 * @param [in] isRebalanced Un booléen qui dicte s'il faut utiliser des ajouts avec ou sans rééquilibrage
 *
 * @pre Les mots sont composés des caractères ASCII (128 possibilités) encodé sur 8 bits
 *
 * La liste de mots donné dans l'entrée standard doit être sous forme d'un mot par ligne. La sortie standard contiendra
 * le Trie Hybride correspondant (construit par ajouts successifs des mots donnés) sous format JSON.
 *
 */
void insererTH(bool isRebalanced);

/**
 * @brief Lit le Patricia-Trie depuis le fichier JSON donné en argument, supprime les mots lus depuis l'entrée standard
 * du trie, reenregistre le Patricia-Trie dans le fichier donné en argument sous format JSON
 *
 * @param [in] path Une chaine de caractères (nul terminé) constituant le chemin vers un fichier JSON d'un Patricia-Trie
 *
 * @pre Les mots sont composés des caractères ASCII (128 possibilités) encodé sur 8 bits
 *
 * La liste de mots donné dans l'entrée standard doit être sous forme d'un mot par ligne.
 *
 */
void suppressionPT(const char *path);

/**
 * @brief Lit le Trie Hybride depuis le fichier JSON donné en argument, supprime les mots lus depuis l'entrée standard
 * du trie, reenregistre le Trie Hybride dans le fichier donné en argument sous format JSON
 *
 * @param [in] path Une chaine de caractères (nul terminé) constituant le chemin vers un fichier JSON d'un Trie Hybride
 *
 * @pre Les mots sont composés des caractères ASCII (128 possibilités) encodé sur 8 bits
 *
 * La liste de mots donné dans l'entrée standard doit être sous forme d'un mot par ligne.
 *
 */
void suppressionTH(const char *path);

/**
 * @brief Lit le Patricia-Trie depuis le fichier JSON donné en argument, liste les mots qu'il contient sur la sortie
 * standard
 *
 * @param [in] path Une chaine de caractères (nul terminé) constituant le chemin vers un fichier JSON d'un Patricia-Trie
 *
 * La liste de mots écrit sur la sortie standard est sous forme d'un mot par ligne.
 *
 */
void listeMotsMainPT(const char *path);

/**
 * @brief Lit le Trie Hybride depuis le fichier JSON donné en argument, liste les mots qu'il contient sur la sortie
 * standard
 *
 * @param [in] path Une chaine de caractères (nul terminé) constituant le chemin vers un fichier JSON d'un Trie Hybride
 *
 * La liste de mots écrit sur la sortie standard est sous forme d'un mot par ligne.
 *
 */
void listeMotsMainTH(const char *path);

/**
 * @brief Lit le Patricia-Trie depuis le fichier JSON donné en argument, écrit sa profondeur moyenne sur la sortie
 * standard
 *
 * @param [in] path Une chaine de caractères (nul terminé) constituant le chemin vers un fichier JSON d'un Patricia-Trie
 *
 * La profondeur moyenne écrit est un flottant et ne contient pas de retour à la ligne.
 *
 */
void profondeurMoyenneMainPT(const char *path);

/**
 * @brief Lit le Trie Hybride depuis le fichier JSON donné en argument, écrit sa profondeur moyenne sur la sortie
 * standard
 *
 * @param [in] path Une chaine de caractères (nul terminé) constituant le chemin vers un fichier JSON d'un Trie Hybride
 *
 * La profondeur moyenne écrit est un flottant et ne contient pas de retour à la ligne.
 *
 */
void profondeurMoyenneMainTH(const char *path);

/**
 * @brief Lit le Patricia-Trie depuis le fichier JSON donné en argument ainsi qu'un mot, écrit le nombre de mots pour
 * lequel le mot donné est le prefixe sur la sortie standard
 *
 * @param [in] path Une chaine de caractères (nul terminé) constituant le chemin vers un fichier JSON d'un Patricia-Trie
 * @param [in] cle Une chaine de caractères (nul terminé) constituant une clé/mot, caractères ASCII (128 possibilités)
 *
 * Le nombre de préfixe écrit est un entier et ne contient pas de retour à la ligne.
 *
 */
void prefixeMainPT(const char *path, const char *cle);

/**
 * @brief Lit le Trie Hybride depuis le fichier JSON donné en argument ainsi qu'un mot, écrit le nombre de mots pour
 * lequel le mot donné est le prefixe sur la sortie standard
 *
 * @param [in] path Une chaine de caractères (nul terminé) constituant le chemin vers un fichier JSON d'un Trie Hybride
 * @param [in] cle Une chaine de caractères (nul terminé) constituant une clé/mot, caractères ASCII (128 possibilités)
 *
 * Le nombre de préfixe écrit est un entier et ne contient pas de retour à la ligne.
 *
 */
void prefixeMainTH(const char *path, const char *cle);

/**
 * @brief Lit les deux Patricia-Trie depuis les fichiers JSON donnés en argument, écrit le trie résultant de leur
 * fusion sur la sortie standard
 *
 * @param [in] path1 Une chaine de caractères (nul terminé) constituant le chemin vers un fichier JSON d'un
 * Patricia-Trie
 * @param [in] path2 Une chaine de caractères (nul terminé) constituant le chemin vers un fichier JSON d'un
 * Patricia-Trie
 *
 */
void fusionMainPT(const char *path1, const char *path2);

/**
 * @brief Lit les deux Tries Hybrides depuis les fichiers JSON donnés en argument, écrit le trie résultant de leur
 * fusion sur la sortie standard
 *
 * @param [in] path1 Une chaine de caractères (nul terminé) constituant le chemin vers un fichier JSON d'un Trie Hybride
 * @param [in] path2 Une chaine de caractères (nul terminé) constituant le chemin vers un fichier JSON d'un Trie Hybride
 *
 */
void fusionMainTH(const char *path1, const char *path2);

#endif
