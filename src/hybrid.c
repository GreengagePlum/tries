/**
 * @file hybrid.c
 * @author Efe ERKEN (efe.erken@etu.sorbonne-universite.fr)
 * @brief Fichier source contenant les corps des fonctions pour le Trie Hybride
 * @version 0.2
 * @date 2024-11-18
 *
 * @copyright Copyright (C) 2024 Efe ERKEN
 *
 */

#include "hybrid.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char prem(const char *cle)
{
    assert(cle != NULL && "Cle donné n'existe pas");
    return *cle;
}

const char *reste(const char *cle)
{
    assert(cle != NULL && "Cle donné n'existe pas");
    if (*cle == '\0')
        return NULL;
    return cle + 1;
}

char car(const char *cle, size_t i)
{
    assert(0 < i && "L'index donné ne doit pas déborder la clé");
    assert(lgueur(cle) >= i && "L'index donné ne doit pas déborder la clé");
    return *(cle + i - 1);
}

size_t lgueur(const char *cle)
{
    assert(cle != NULL && "Clé donné n'existe pas");
    return strlen(cle);
}

TrieHybride newTH(void)
{
    return NULL;
}

TrieHybride allocTH(void)
{
    TrieHybride newth = malloc(sizeof(*newth));
    if (!newth)
    {
        fprintf(stderr, "Erreur, malloc dans ajoutTH");
        exit(1);
    }
    return newth;
}

TrieHybride ajoutTH(TrieHybride th, const char *restrict cle, int v)
{
    assert(v != 0 && "Valeur donné pour l'insértion doit être non nul");
    size_t lgr = lgueur(cle);
    if (lgr == 0)
        return th;
    char p = prem(cle);
    if (!th)
    {
        TrieHybride newth = allocTH();
        newth->label = p;
        newth->inf = NULL;
        newth->sup = NULL;
        if (lgr == 1)
        {
            newth->value = v;
            newth->eq = NULL;
        }
        else
        {
            newth->value = VALVIDE;
            newth->eq = ajoutTH(newTH(), reste(cle), v);
        }
        return newth;
    }
    if (p < th->label)
        th->inf = ajoutTH(th->inf, cle, v);
    else if (p > th->label)
        th->sup = ajoutTH(th->sup, cle, v);
    else
        th->eq = ajoutTH(th->eq, reste(cle), v);
    if (lgr == 1 && !th->value)
    {
        th->value = v;
    }
    return th;
}

/**
 * @private
 */
typedef enum presence_enfants
{
    INF = 1,     /**< Seulement l'enfant Inf est présent */
    SUP = 2,     /**< Seulement l'enfant Sup est présent */
    INFSUP = 3,  /**< Les deux enfants Inf et Sup sont présents */
    AUCUNENF = 0 /**< Aucun des deux enfants Inf et Sup sont présents */
} PresenceEnfants;

/**
 * @private
 *
 * @brief Calcule une valeur qui indique lesquels des enfants du Trie Hybride sont présents
 *
 * @param [in] th Un pointeur vers le Trie Hybride à calculer les enfants
 * @return Une valeur dans l'intervalle [0; 3] pour indiquer lesquels des enfants sont présents à part l'enfant Eq
 *
 * @pre Le Trie Hybride donné est non vide (non nul)
 *
 * L'enfant Eq n'est pas tenu en compte, il faut vérifier cela à part.
 *
 */
PresenceEnfants determine_enfants(TrieHybride th)
{
    return (!!th->sup << 1) | (!!th->inf);
}

/**
 * @private
 *
 * @brief Supprime le noeud donné si possible et réorganise les sous arbres
 *
 * @param [in,out] th Un pointeur vers le noeud du Trie Hybride à essayer de supprimer
 * @param [out] didDelete Un flag mis à vrai si une supression a bien pu s'effectuer
 * @return Un pointeur vers le Trie Hybride avec potentiellemnt la clé supprimé et reorganisé
 *
 * @pre didDelete a été alloué par l'appelant
 *
 * Cette fonction aide à la suppression d'une clé d'un Trie Hybride avec une manipulation délicate.
 * Il y a plusieurs cas de traitement.
 *
 */
TrieHybride supprTH_essaye_delete_reorg(TrieHybride th, bool *didDelete)
{
    if (th->eq || th->value)
    {
        return th;
    }
    TrieHybride tmp;
    switch (determine_enfants(th))
    {
    case INFSUP:
        tmp = th->inf;
        while (tmp->sup)
            tmp = tmp->sup;
        tmp->sup = th->sup;
        tmp = th->inf;
        break;
    case INF:
        tmp = th->inf;
        break;
    case SUP:
        tmp = th->sup;
        break;
    case AUCUNENF:
        tmp = NULL;
        break;
    default:
        fprintf(stderr, "Erreur, calcul présence des enfants malheureux");
        exit(1);
    }
    free(th);
    th = tmp;
    *didDelete = true;
    return th;
}

/**
 * @private
 *
 * @brief Supprime la clé donné avec libération des noeuds si possible et réorganise les sous arbres
 *
 * @param [in,out] th Un pointeur vers le Trie Hybride à traiter
 * @param [in] cle Une chaine de caractères constituant une clé
 * @param [in,out] didDelete Un flag mis à vrai si une supression a bien pu s'effectuer
 * @return Un pointeur vers le Trie Hybride avec potentiellemnt la clé supprimé et reorganisé
 *
 * @pre didDelete a été alloué par l'appelant
 *
 */
TrieHybride supprTH_rec(TrieHybride th, const char *restrict cle, bool *didDelete)
{
    if (!th)
        return th;
    char p = prem(cle);
    if (p == '\0')
        return th;
    if (p < th->label)
    {
        th->inf = supprTH_rec(th->inf, cle, didDelete);
    }
    else if (p > th->label)
    {
        th->sup = supprTH_rec(th->sup, cle, didDelete);
    }
    else
    {
        th->eq = supprTH_rec(th->eq, reste(cle), didDelete);
    }
    if (lgueur(cle) == 1)
    {
        if (th->value)
        {
            th->value = VALVIDE;
            th = supprTH_essaye_delete_reorg(th, didDelete);
        }
    }
    else if (didDelete)
    {
        assert(lgueur(cle) > 1 && "Etat incorrecte, longueur de la clé doit être strictement positif");
        th = supprTH_essaye_delete_reorg(th, didDelete);
    }
    return th;
}

TrieHybride supprTH(TrieHybride th, const char *restrict cle)
{
    bool didDelete = false;
    return supprTH_rec(th, cle, &didDelete);
}

TrieHybride deleteTH_rec(TrieHybride th)
{
    if (!th)
        return NULL;
    th->inf = deleteTH_rec(th->inf);
    th->eq = deleteTH_rec(th->eq);
    th->sup = deleteTH_rec(th->sup);
    free(th);
    return NULL;
}

void deleteTH(TrieHybride *th)
{
    if (!th)
        return;
    *th = deleteTH_rec(*th);
}
