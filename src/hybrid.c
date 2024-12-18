/**
 * @file hybrid.c
 * @author Efe ERKEN (efe.erken@etu.sorbonne-universite.fr)
 * @brief Fichier source contenant les corps des fonctions pour le Trie Hybride
 * @version 0.6
 * @date 2024-12-18
 *
 * @copyright Copyright (C) 2024 Efe ERKEN
 *
 */

#include "hybrid.h"
#include "cJSON.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

/**
 * @private
 *
 * @def MAX3
 *
 * @brief Trouve le moximum entre 3 valeurs
 *
 * ATTENTION à ne pas faire des appels à des fonction en paramètre de ce macro à risque qu'il appelle PLUSIEURS fois vos
 * fonctions ! Passez seulement des valeurs (variables).
 *
 */
#define MAX3(a, b, c) ((a) > (b) ? ((a) > (c) ? (a) : (c)) : ((b) > (c) ? (b) : (c)))

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

TrieHybride *newTH(void)
{
    return NULL;
}

TrieHybride *allocTH(void)
{
    TrieHybride *newth = malloc(sizeof(*newth));
    if (!newth)
    {
        fprintf(stderr, "Erreur, malloc dans allocTH");
        exit(1);
    }
    return newth;
}

TrieHybride *ajoutTH(TrieHybride *th, const char *restrict cle, int v)
{
    assert(v != 0 && "Valeur donné pour l'insértion doit être non nul");
    size_t lgr = lgueur(cle);
    if (lgr == 0)
        return th;
    char p = prem(cle);
    if (!th)
    {
        TrieHybride *newth = allocTH();
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
    if (lgr == 1 && th->label == p && !th->value)
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
PresenceEnfants determine_enfants(const TrieHybride *th)
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
TrieHybride *supprTH_essaye_delete_reorg(TrieHybride *th, bool *didDelete)
{
    if (th->eq || th->value)
    {
        return th;
    }
    TrieHybride *tmp;
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
TrieHybride *supprTH_rec(TrieHybride *th, const char *restrict cle, bool *didDelete)
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

TrieHybride *supprTH(TrieHybride *th, const char *restrict cle)
{
    bool didDelete = false;
    return supprTH_rec(th, cle, &didDelete);
}

TrieHybride *deleteTH_rec(TrieHybride *th)
{
    if (!th)
        return NULL;
    th->inf = deleteTH_rec(th->inf);
    th->eq = deleteTH_rec(th->eq);
    th->sup = deleteTH_rec(th->sup);
    free(th);
    return NULL;
}

void deleteTH(TrieHybride **th)
{
    if (!th)
        return;
    *th = deleteTH_rec(*th);
}

bool rechercheTH(const TrieHybride *th, const char *restrict cle)
{
    if (!th)
        return false;
    size_t lgr = lgueur(cle);
    if (lgr == 0)
        return false;
    bool res;
    char p = prem(cle);
    if (p < th->label)
        res = rechercheTH(th->inf, cle);
    else if (p > th->label)
        res = rechercheTH(th->sup, cle);
    else
        res = rechercheTH(th->eq, reste(cle));
    if (lgr == 1)
        return res || (th->label == p && th->value);
    return res;
}

size_t comptageMotsTH(const TrieHybride *th)
{
    if (!th)
        return 0;
    if (th->value)
        return 1 + comptageMotsTH(th->inf) + comptageMotsTH(th->eq) + comptageMotsTH(th->sup);
    return comptageMotsTH(th->inf) + comptageMotsTH(th->eq) + comptageMotsTH(th->sup);
}

typedef struct stack
{
    size_t cap;
    size_t sz;
    char *tab;
} Stack;

Stack newStack(size_t capacity)
{
    assert(capacity > 0 && "Capacité donné doit être strictement positive");
    Stack s;
    s.cap = capacity;
    s.sz = 0;
    s.tab = malloc(capacity * sizeof(*s.tab));
    if (!s.tab)
    {
        fprintf(stderr, "Erreur, malloc dans listeMots");
        exit(1);
    }
    return s;
}

void freeStack(Stack s)
{
    free(s.tab);
}

void pushStack(Stack *s, char c)
{
    assert(s->sz + 1 <= s->cap && "Capacité de la pile de caractères dépassé");
    s->tab[s->sz++] = c;
}

char popStack(Stack *s)
{
    assert(s->sz > 0 && "Tentative de dépilement sur une pile de caractères vide");
    return s->tab[--s->sz];
}

char *snapshotStack(Stack *s)
{
    if (!s)
        return NULL;
    if (s->sz == 0)
        return NULL;
    char *ret = malloc((s->sz + 1) * sizeof(*ret));
    if (!ret)
    {
        fprintf(stderr, "Erreur, malloc dans snapshotStack");
        exit(1);
    }
    memcpy(ret, s->tab, s->sz);
    ret[s->sz] = '\0';
    return ret;
}

const char *readStack(Stack *s)
{
    if (!s)
        return NULL;
    if (s->sz == 0)
        return NULL;
    s->tab[s->sz] = '\0';
    return s->tab;
}

void listeMotsTH_rec(const TrieHybride *th, Stack *s, char **tab, size_t *idx)
{
    if (!th)
        return;
    listeMotsTH_rec(th->inf, s, tab, idx);
    pushStack(s, th->label);
    if (th->value)
    {
        tab[*idx] = snapshotStack(s);
        (*idx)++;
    }
    listeMotsTH_rec(th->eq, s, tab, idx);
    popStack(s);
    listeMotsTH_rec(th->sup, s, tab, idx);
}

char **listeMotsTH(const TrieHybride *th)
{
    if (!th)
        return NULL;
    size_t sz = comptageMotsTH(th);
    char **tab = malloc((sz + 1) * sizeof(*tab));
    if (!tab)
    {
        fprintf(stderr, "Erreur, malloc dans listeMots");
        exit(1);
    }
    tab[sz] = NULL;
    Stack s = newStack(hauteurTH(th) + 1);
    sz = 0;
    listeMotsTH_rec(th, &s, tab, &sz);
    assert(s.sz == 0 && "La pile des caractères doit être vide à cet instant");
    freeStack(s);
    return tab;
}

void deleteListeMotsTH(char **tab)
{
    if (!tab)
        return;
    for (char **i = tab; *i; i++)
    {
        free(*i);
    }
    free(tab);
}

int comptageNilTH(const TrieHybride *th)
{
    if (!th)
        return 1;
    return comptageNilTH(th->inf) + comptageNilTH(th->eq) + comptageNilTH(th->sup);
}

ssize_t hauteurTH(const TrieHybride *th)
{
    if (!th)
        return -1;
    ssize_t inf = hauteurTH(th->inf);
    ssize_t eq = hauteurTH(th->eq);
    ssize_t sup = hauteurTH(th->sup);
    return 1 + MAX3(inf, eq, sup);
}

void profondeurMoyenneTH_rec(const TrieHybride *th, int depth, int *sum, int *count)
{
    if (!th)
        return;
    if (!th->eq && determine_enfants(th) == AUCUNENF)
    {
        (*sum) += depth;
        (*count)++;
        return;
    }
    profondeurMoyenneTH_rec(th->inf, depth + 1, sum, count);
    profondeurMoyenneTH_rec(th->eq, depth + 1, sum, count);
    profondeurMoyenneTH_rec(th->sup, depth + 1, sum, count);
}

int profondeurMoyenneEntTH(const TrieHybride *th)
{
    int sum = 0, count = 0;
    profondeurMoyenneTH_rec(th, 0, &sum, &count);
    if (count == 0)
        return -1;
    assert(count > 0 && "Le compte des feuilles doit être strictement positif");
    assert(sum >= 0 && "La somme des profondeurs doit être positif ou nul");
    return sum / count;
}

double profondeurMoyenneTH(const TrieHybride *th)
{
    int sum = 0, count = 0;
    profondeurMoyenneTH_rec(th, 0, &sum, &count);
    if (count == 0)
        return -1;
    assert(count > 0 && "Le compte des feuilles doit être strictement positif");
    assert(sum >= 0 && "La somme des profondeurs doit être positif ou nul");
    return (double)sum / count;
}

int prefixeTH_rec(const TrieHybride *th)
{
    if (!th)
        return 0;
    return !!th->value + prefixeTH_rec(th->inf) + prefixeTH_rec(th->eq) + prefixeTH_rec(th->sup);
}

int prefixeTH(const TrieHybride *th, const char *cle)
{
    if (!th)
        return 0;

    const TrieHybride *lastNode = NULL;
    const TrieHybride *subtree = th;
    const char *r = cle;
    char p;
    /* Advance until the subtree that interests us */
    while (subtree)
    {
        p = prem(r);
        if (p == '\0')
            break;

        lastNode = subtree;
        if (p < subtree->label)
            subtree = subtree->inf;
        else if (p > subtree->label)
            subtree = subtree->sup;
        else
        {
            subtree = subtree->eq;
            r = reste(r);
        }
    }

    /* Yeah not too proud of this one */
    return (!(*r) && lastNode && lastNode->value) + prefixeTH_rec(subtree);
}

cJSON *constructJSONTH(const TrieHybride *th)
{
    if (!th)
    {
        cJSON *null = cJSON_CreateNull();
        if (!null)
        {
            fprintf(stderr, "Erreur, cJSON_CreateNull dans constructJSONTH");
            exit(1);
        }
        return null;
    }
    cJSON *obj = cJSON_CreateObject();
    if (!obj)
    {
        fprintf(stderr, "Erreur, cJSON_CreateObject dans constructJSONTH");
        exit(1);
    }
    char buf[2] = {th->label, 0};
    if (!cJSON_AddStringToObject(obj, "char", buf))
    {
        fprintf(stderr, "Erreur, cJSON_AddStringToObject dans constructJSONTH");
        exit(1);
    }
    if (!cJSON_AddBoolToObject(obj, "is_end_of_word", th->value))
    {
        fprintf(stderr, "Erreur, cJSON_AddBoolToObject dans constructJSONTH");
        exit(1);
    }

    cJSON *child;
    child = constructJSONTH(th->inf);
    if (!cJSON_AddItemToObject(obj, "left", child))
    {
        fprintf(stderr, "Erreur, cJSON_AddItemToObject dans constructJSONTH");
        exit(1);
    }
    child = constructJSONTH(th->eq);
    if (!cJSON_AddItemToObject(obj, "middle", child))
    {
        fprintf(stderr, "Erreur, cJSON_AddItemToObject dans constructJSONTH");
        exit(1);
    }
    child = constructJSONTH(th->sup);
    if (!cJSON_AddItemToObject(obj, "right", child))
    {
        fprintf(stderr, "Erreur, cJSON_AddItemToObject dans constructJSONTH");
        exit(1);
    }

    return obj;
}

char *printJSONTH(const TrieHybride *th)
{
    cJSON *js = constructJSONTH(th);
    char *str = cJSON_Print(js);
    /* TODO: Check if cJSON is null and skip output completely (vanity) */
    cJSON_Delete(js);
    return str;
}

TrieHybride *parseJSONTH_rec(const cJSON *json)
{
    if (cJSON_IsNull(json))
        return NULL;

    TrieHybride *th = allocTH();
    cJSON *obj;

    obj = cJSON_GetObjectItemCaseSensitive(json, "char");
    assert(cJSON_IsString(obj) && "'char' element has to be a JSON string element");
    th->label = obj->valuestring[0];
    obj = cJSON_GetObjectItemCaseSensitive(json, "is_end_of_word");
    assert(cJSON_IsBool(obj) && "'is_end_of_word' element has to be a JSON bool element");
    th->value = cJSON_IsTrue(obj);

    obj = cJSON_GetObjectItemCaseSensitive(json, "left");
    assert((cJSON_IsNull(obj) || cJSON_IsObject(obj)) && "'left' element has to be a JSON null or object element");
    th->inf = parseJSONTH_rec(obj);

    obj = cJSON_GetObjectItemCaseSensitive(json, "middle");
    assert((cJSON_IsNull(obj) || cJSON_IsObject(obj)) && "'middle' element has to be a JSON null or object element");
    th->eq = parseJSONTH_rec(obj);

    obj = cJSON_GetObjectItemCaseSensitive(json, "right");
    assert((cJSON_IsNull(obj) || cJSON_IsObject(obj)) && "'right' element has to be a JSON null or object element");
    th->sup = parseJSONTH_rec(obj);

    return th;
}

TrieHybride *parseJSONTH(const char *json, size_t sz)
{
    cJSON *obj = cJSON_ParseWithLength(json, sz);
    if (!obj)
    {
        fprintf(stderr, "Erreur, cJSON_ParseWithLength dans parseJSONTH");
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        exit(1);
    }
    TrieHybride *th = parseJSONTH_rec(obj);
    cJSON_Delete(obj);
    return th;
}

TrieHybride *fusionTH_rec(TrieHybride *restrict th1, const TrieHybride *restrict th2, Stack *s)
{
    if (!th2)
        return th1;
    th1 = fusionTH_rec(th1, th2->inf, s);
    pushStack(s, th2->label);
    if (th2->value)
    {
        const char *str = readStack(s);
        assert(str && "La chaine lu ne peut pas être nul ici");
        th1 = ajoutTH(th1, str, VALFIN);
    }
    th1 = fusionTH_rec(th1, th2->eq, s);
    popStack(s);
    th1 = fusionTH_rec(th1, th2->sup, s);
    return th1;
}

TrieHybride *fusionTH(TrieHybride **restrict th1, const TrieHybride *restrict th2)
{
    assert(th1 && "Contract violated, null pointer passed in");
    TrieHybride *th = *th1;
    if (th2)
    {
        Stack s = newStack(hauteurTH(th2) + 1);
        th = fusionTH_rec(*th1, th2, &s);
        assert(s.sz == 0 && "La pile des caractères doit être vide à cet instant");
        freeStack(s);
    }
    *th1 = NULL;
    return th;
}

TrieHybride *fusionCopieTH(const TrieHybride *restrict th1, const TrieHybride *restrict th2)
{
    TrieHybride *th = newTH();
    th = fusionTH(&th, th1);
    th = fusionTH(&th, th2);
    return th;
}

int indirectionLevel(TrieHybride *th)
{
    if (!th)
        return 0;

    if (determine_enfants(th) == AUCUNENF)
        return 0;

    return 1 + indirectionLevel(th->inf) + indirectionLevel(th->sup);
}

/**
 * @private
 *
 * @brief Effectue une rotation droite pour équilibrer
 *
 * @pre Le Trie Hybride donné est non vide (non nul)
 *
 */
TrieHybride *rotateRight(TrieHybride *y)
{
    assert(y && "Given Hybrid Trie can't be NULL");
    TrieHybride *x = y->inf;
    TrieHybride *T = x->sup;

    x->sup = y;
    y->inf = T;

    return x;
}

/**
 * @private
 *
 * @brief Effectue une rotation gauche pour équilibrer
 *
 * @pre Le Trie Hybride donné est non vide (non nul)
 *
 */
TrieHybride *rotateLeft(TrieHybride *x)
{
    assert(x && "Given Hybrid Trie can't be NULL");
    TrieHybride *y = x->sup;
    TrieHybride *T = y->inf;

    y->inf = x;
    x->sup = T;

    return y;
}

TrieHybride *rebalance(TrieHybride *th)
{
    assert(th && "Given Hybrid Trie can't be NULL");
    int balanceFactor = indirectionLevel(th->inf) - indirectionLevel(th->sup);

    /* Left heavy */
    if (balanceFactor >= 1)
    {
        if (indirectionLevel(th->inf->inf) >= indirectionLevel(th->inf->sup))
        {
            /* Single right rotation */
            return rotateRight(th);
        }
        else
        {
            /* Left-right rotation */
            th->inf = rotateLeft(th->inf);
            return rotateRight(th);
        }
    }

    /* Right heavy */
    if (balanceFactor <= -1)
    {
        if (indirectionLevel(th->sup->sup) >= indirectionLevel(th->sup->inf))
        {
            /* Single left rotation */
            return rotateLeft(th);
        }
        else
        {
            /* Right-left rotation */
            th->sup = rotateRight(th->sup);
            return rotateLeft(th);
        }
    }

    /* Node is balanced */
    return th;
}

TrieHybride *ajoutReequilibreTH(TrieHybride *th, const char *restrict cle, int v)
{
    assert(v != 0 && "Valeur donné pour l'insértion doit être non nul");
    size_t lgr = lgueur(cle);
    if (lgr == 0)
        return th;
    char p = prem(cle);
    if (!th)
    {
        TrieHybride *newth = allocTH();
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
            newth->eq = ajoutReequilibreTH(newTH(), reste(cle), v);
        }
        return newth;
    }
    if (p < th->label)
        th->inf = ajoutReequilibreTH(th->inf, cle, v);
    else if (p > th->label)
        th->sup = ajoutReequilibreTH(th->sup, cle, v);
    else
        th->eq = ajoutReequilibreTH(th->eq, reste(cle), v);
    if (lgr == 1 && th->label == p && !th->value)
    {
        th->value = v;
    }
    return rebalance(th);
}
