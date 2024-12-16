#ifndef HYBRID_H
#define HYBRID_H
/**
 * @file hybrid.h
 * @author Efe ERKEN (efe.erken@etu.sorbonne-universite.fr)
 * @brief Fichier d'entête contenant les déclaration des fonctions pour le Trie Hybride
 * @version 0.5
 * @date 2024-11-18
 *
 * @copyright Copyright (C) 2024 Efe ERKEN
 *
 */

#include <stdbool.h>
#include <stddef.h>
#include <sys/types.h>

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
    long value;  /**< Indicateur de fin de mot si non nul */
    char label; /**< Un caractère d'une clé stocké dans le trie */
    struct trie_hybride *inf, *eq, *sup;
} TrieHybride;

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
TrieHybride *newTH(void);

/**
 * @brief Ajoute une clé dans le Trie Hybride donné
 *
 * @param [in,out] th Un pointeur vers le Trie Hybride à insérer la clé
 * @param [in] cle Une chaine de caractères constituant une clé
 * @param [in] v Une valeur non nul pour indiquer la fin du mot, peut être le numéro d'insértion, le constant @c VALFIN
 * peut être utilisé
 * @return Un pointeur vers le Trie Hybride avec la clé ajouté
 *
 * @pre La clé est terminé par un caractère nul
 * @pre La clé est composé des caractères ASCII (128 possibilités) encodé sur 8 bits
 *
 */
TrieHybride *ajoutTH(TrieHybride *th, const char *restrict cle, int v);

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
 * @internal Le constant @c VALVIDE peut être utilisé pour tester si un noeud n'est pas terminal
 *
 */
TrieHybride *supprTH(TrieHybride *th, const char *restrict cle);

/**
 * @brief Libère l'espace occupé par le Trie Hybride donné
 *
 * @param [in,out] th Un pointeur de pointeur vers le Trie Hybride à libérer. Mis à nul à la fin.
 *
 * Libère en utilisant un parcours en postfix. Le trie donné en paramètre peut être nul ou vide ou déjà désalloué, ces
 * cas sont gérés.
 *
 */
void deleteTH(TrieHybride **th);

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
bool rechercheTH(const TrieHybride *th, const char *restrict cle);

/**
 * @brief Compte le nombre de mots dans le Trie Hybride donné
 *
 * @param [in] th Un pointeur vers le Trie Hybride à parcourir
 * @return Un entier non signé indiquant le nombre de clés/mots trouvés
 *
 */
size_t comptageMotsTH(const TrieHybride *th);

/**
 * @brief Liste les mots du trie dans l'ordre alphabétique
 *
 * @param [in] th Un pointeur vers le Trie Hybride à parcourir
 * @return Un tableau, de dernier élément nul, de chaines de caractères avec tous les mots du trie en ordre
 * alphabétique, pointeur nul est retourné au lieu d'un tableau dans le cas le trie donné est vide
 *
 * @post Le tableau retourné doit être libéré avec la fonction `deleteListeMots()`
 *
 */
char **listeMotsTH(const TrieHybride *th);

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
int comptageNilTH(const TrieHybride *th);

/**
 * @brief Calcule la hauteur du Trie Hybride donné
 *
 * @param [in] th Un pointeur vers le Trie Hybride à parcourir
 * @return Un entier non négatif indiquant la hauteur de l'arbre engandré par le trie
 *
 */
ssize_t hauteurTH(const TrieHybride *th);

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
int profondeurMoyenneEntTH(const TrieHybride *th);

/**
 * @brief Calcule la profondeur moyenne du Trie Hybride donné
 *
 * @param [in] th Un pointeur vers le Trie Hybride à parcourir
 * @return Un flottant indiquant la profondeur moyenne de l'arbre engandré par le trie
 *
 * Le retour est négatif quand le trie donné est vide.
 *
 */
double profondeurMoyenneTH(const TrieHybride *th);

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
int prefixeTH(const TrieHybride *th, const char *cle);

/**
 * @brief Sérialise le Trie Hybride donné sous format JSON
 *
 * @param [in] th Un pointeur vers le Trie Hybride à parcourir
 * @return Une chaine de caractères contenant la version sérialisé en JSON du Trie Hybride
 *
 * @post C'est à l'appellant de désallouer la chaine retourné
 *
 */
char *printJSONTH(const TrieHybride *th);

/**
 * @brief Désérialise le JSON en Trie Hybride
 *
 * @param [in] json Une chaine de caractères contenant un Trie Hybride sous format JSON
 * @param [in] sz La taille en octets de la chaine json
 * @return Un pointeur vers le Trie Hybride créé
 *
 * @post C'est à l'appellant de désallouer le Trie Hybride retourné
 *
 * C'est une désérialisation dit "one-to-one", c'est à dire la représentation interne en mémoire du trie donné sous
 * forme de JSON ne passe en aucun cas par d'autres primitives de Trie Hybride. Cela est fait dans le but d'éviter la
 * création d'un arbre différent dû à la reconstruction. Le Trie Hybride créé correspond exactement à la disposition
 * donné en JSON.
 *
 */
TrieHybride *parseJSONTH(const char *json, size_t sz);

/**
 * @brief Fusionne deux Tries Hybrides donnés (semi "in place")
 *
 * @param [in,out] th1 Un pointeur de pointeur vers le Trie Hybride à fusionner
 * @param [in] th2 Un pointeur vers le Trie Hybride à fusionner
 * @return Un pointeur vers le Trie Hybride résultant de la fusion
 *
 * @pre @a th1 n'est pas nul
 * @pre Les zones mémoire des deux tries données sont bien distinctes
 *
 * @post @a th1 pointe sur nul
 *
 * Le deuxième Trie Hybride @a th2 est fusionné au premier Trie Hybride @a th1. C'est une fusion dit semi "in place",
 * sans utiliser de l'espace mémoire secondaire (pas d'allocation de tout un nouveau trie de zéro). Les noeuds de @a th1
 * sont réutilisés pour construire un troisième trie. De ce fait, @a th1 pointe sur nul à la fin (tous ses noeuds
 * ont été forcément consommés). Il est donc recommandé de réassigner le pointeur @a th1 passé en argument au retour de
 * cette fonction. @a th2 reste identique et n'est accédé qu'en lecture. Le trie pointé par @a th2 est donc à désallouer
 * par l'appellant avec @c deleteTH contrairement à @a th1.
 *
 */
TrieHybride *fusionTH(TrieHybride **restrict th1, const TrieHybride *restrict th2);

/**
 * @brief Fusionne deux Tries Hybrides donnés (par recopie)
 *
 * @param [in] th1 Un pointeur vers le Trie Hybride à fusionner
 * @param [in] th2 Un pointeur vers le Trie Hybride à fusionner
 * @return Un pointeur vers le Trie Hybride résultant de la fusion
 *
 * Le deuxième Trie Hybride @a th2 est fusionné au premier Trie Hybride @a th1.
 * Les deux tries sont seulement parcourus en lecture. Le Trie Hybride retourné consiste seulement des noeuds
 * nouvellement alloué indépendants.
 * @a th1 et @a th2 restent comme tel et doivent être désalloués par l'appellant.
 *
 */
TrieHybride *fusionCopieTH(const TrieHybride *restrict th1, const TrieHybride *restrict th2);

#endif
