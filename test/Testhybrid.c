#include "hybrid.h"
#include "unity.h"
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define PATH_MAX 1024

const char *const basic_example =
    "A quel genial professeur de dactylographie sommes nous redevables de la superbe phrase ci "
    "dessous, un modele du genre, que toute dactylo connait par coeur puisque elle fait appel a "
    "chacune des touches du clavier de la machine a ecrire ?";

void setUp(void)
{
    // set stuff up here
}

void tearDown(void)
{
    // clean stuff up here
}

/* Black box test */
void test_f_prem(void)
{
    const char *cle;

    cle = "";
    TEST_ASSERT_EQUAL_CHAR('\0', prem(cle));

    cle = "a";
    TEST_ASSERT_EQUAL_CHAR('a', prem(cle));

    cle = "A";
    TEST_ASSERT_EQUAL_CHAR('A', prem(cle));

    cle = "z";
    TEST_ASSERT_EQUAL_CHAR('z', prem(cle));

    cle = "Z";
    TEST_ASSERT_EQUAL_CHAR('Z', prem(cle));

    cle = "1";
    TEST_ASSERT_EQUAL_CHAR('1', prem(cle));

    cle = "0";
    TEST_ASSERT_EQUAL_CHAR('0', prem(cle));

    cle = "$";
    TEST_ASSERT_EQUAL_CHAR('$', prem(cle));

    cle = ".";
    TEST_ASSERT_EQUAL_CHAR('.', prem(cle));

    cle = "beej:";
    TEST_ASSERT_EQUAL_CHAR('b', prem(cle));

    cle = "\00Hello World";
    TEST_ASSERT_EQUAL_CHAR('\0', prem(cle));

    cle = "ElCaTrAz-DeeZ;";
    TEST_ASSERT_EQUAL_CHAR('E', prem(cle));

    cle = "\x1B\x20";
    TEST_ASSERT_EQUAL_CHAR(0x1B, prem(cle));

    cle = "\x9";
    TEST_ASSERT_EQUAL_CHAR(0x9, prem(cle));

    cle = "&var";
    TEST_ASSERT_EQUAL_CHAR('&', prem(cle));

    cle = "(bloom)";
    TEST_ASSERT_EQUAL_CHAR('(', prem(cle));

    cle = "\00";
    TEST_ASSERT_EQUAL_CHAR('\0', prem(cle));
}

/* Black box test */
void test_f_reste(void)
{
    const char *cle;

    cle = "";
    TEST_ASSERT_EQUAL_STRING(NULL, reste(cle));

    cle = "a";
    TEST_ASSERT_EQUAL_STRING("\0", reste(cle));

    cle = "beej:";
    TEST_ASSERT_EQUAL_STRING("eej:", reste(cle));

    cle = "\00Hello World";
    TEST_ASSERT_EQUAL_STRING(NULL, reste(cle));

    cle = "ElCaTrAz-DeeZ;";
    TEST_ASSERT_EQUAL_STRING("lCaTrAz-DeeZ;", reste(cle));

    cle = "\x1B\x20";
    TEST_ASSERT_EQUAL_STRING("\x20", reste(cle));

    cle = "\x9";
    TEST_ASSERT_EQUAL_STRING("\0", reste(cle));

    cle = "&var";
    TEST_ASSERT_EQUAL_STRING("var", reste(cle));

    cle = "(bloom)";
    TEST_ASSERT_EQUAL_STRING("bloom)", reste(cle));

    cle = "\00";
    TEST_ASSERT_EQUAL_STRING(NULL, reste(cle));
}

/* Black box test */
void test_f_car(void)
{
    const char *cle = "aA.$3\x1B";
    TEST_ASSERT_EQUAL_CHAR('a', car(cle, 1));
    TEST_ASSERT_EQUAL_CHAR('A', car(cle, 2));
    TEST_ASSERT_EQUAL_CHAR('.', car(cle, 3));
    TEST_ASSERT_EQUAL_CHAR('$', car(cle, 4));
    TEST_ASSERT_EQUAL_CHAR('3', car(cle, 5));
    TEST_ASSERT_EQUAL_CHAR(0x1B, car(cle, 6));
}

/* Black box test */
void test_f_lgueur(void)
{
    const char *cle;

    cle = "aA.$3\x1B";
    TEST_ASSERT(((size_t)6) == lgueur(cle));

    cle = "";
    TEST_ASSERT(((size_t)0) == lgueur(cle));

    cle = "LeBron\0James";
    TEST_ASSERT(((size_t)6) == lgueur(cle));

    cle = "\0";
    TEST_ASSERT(((size_t)0) == lgueur(cle));

    cle = "a";
    TEST_ASSERT(((size_t)1) == lgueur(cle));

    cle = "Hello, World!";
    TEST_ASSERT(((size_t)13) == lgueur(cle));
}

/* White box test
 *
 * case #1:
 * empty insertion
 */
void test_f_ajoutTH_1(void)
{
    const char *cle = "hello";

    TrieHybride *th = newTH();
    th = ajoutTH(th, cle, VALFIN);

    TEST_ASSERT_NOT_NULL(th);
    TrieHybride *tmp = th;
    bool foundEndChar = false;
    while (tmp)
    {
        TEST_ASSERT(strlen(cle) > 0);
        TEST_ASSERT_NULL(tmp->inf);
        TEST_ASSERT_NULL(tmp->sup);
        TEST_ASSERT_EQUAL_CHAR(prem(cle), tmp->label);
        if (tmp->value)
        {
            TEST_ASSERT_EQUAL_CHAR('o', tmp->label);
            foundEndChar = true;
        }

        tmp = tmp->eq;
        cle++;
    }
    TEST_ASSERT_EQUAL_CHAR('\0', *cle);
    TEST_ASSERT_TRUE(foundEndChar);

    deleteTH(&th);
}

/* White box test
 *
 * case #2:
 * full insertion,
 * no relation to existing key,
 * th->sup side insertion
 */
void test_f_ajoutTH_2(void)
{
    const char *cle1 = "hello";
    const char *cle2 = "pro";

    TrieHybride *th = newTH();
    th = ajoutTH(th, cle1, VALFIN);
    th = ajoutTH(th, cle2, VALFIN);

    TrieHybride *tmp = th;
    TEST_ASSERT_NOT_NULL(tmp);
    TEST_ASSERT_EQUAL_CHAR('h', tmp->label);
    TEST_ASSERT_EQUAL(VALVIDE, tmp->value);
    TEST_ASSERT_NULL(tmp->inf);
    TEST_ASSERT_NOT_NULL(tmp->eq);

    tmp = tmp->sup;
    TEST_ASSERT_NOT_NULL(tmp);
    TEST_ASSERT_EQUAL_CHAR('p', tmp->label);
    TEST_ASSERT_EQUAL(VALVIDE, tmp->value);
    TEST_ASSERT_NULL(tmp->inf);
    TEST_ASSERT_NULL(tmp->sup);

    tmp = tmp->eq;
    TEST_ASSERT_NOT_NULL(tmp);
    TEST_ASSERT_EQUAL_CHAR('r', tmp->label);
    TEST_ASSERT_EQUAL(VALVIDE, tmp->value);
    TEST_ASSERT_NULL(tmp->inf);
    TEST_ASSERT_NULL(tmp->sup);

    tmp = tmp->eq;
    TEST_ASSERT_NOT_NULL(tmp);
    TEST_ASSERT_EQUAL_CHAR('o', tmp->label);
    TEST_ASSERT_EQUAL(VALFIN, tmp->value);
    TEST_ASSERT_NULL(tmp->inf);
    TEST_ASSERT_NULL(tmp->eq);
    TEST_ASSERT_NULL(tmp->sup);

    deleteTH(&th);
}

/* White box test
 *
 * case #3:
 * full insertion,
 * no relation to existing key,
 * th->inf side insertion
 */
void test_f_ajoutTH_3(void)
{
    const char *cle1 = "hello";
    const char *cle2 = "bro";

    TrieHybride *th = newTH();
    th = ajoutTH(th, cle1, VALFIN);
    th = ajoutTH(th, cle2, VALFIN);

    TrieHybride *tmp = th;
    TEST_ASSERT_NOT_NULL(tmp);
    TEST_ASSERT_EQUAL_CHAR('h', tmp->label);
    TEST_ASSERT_EQUAL(VALVIDE, tmp->value);
    TEST_ASSERT_NULL(tmp->sup);
    TEST_ASSERT_NOT_NULL(tmp->eq);

    tmp = tmp->inf;
    TEST_ASSERT_NOT_NULL(tmp);
    TEST_ASSERT_EQUAL_CHAR('b', tmp->label);
    TEST_ASSERT_EQUAL(VALVIDE, tmp->value);
    TEST_ASSERT_NULL(tmp->inf);
    TEST_ASSERT_NULL(tmp->sup);

    tmp = tmp->eq;
    TEST_ASSERT_NOT_NULL(tmp);
    TEST_ASSERT_EQUAL_CHAR('r', tmp->label);
    TEST_ASSERT_EQUAL(VALVIDE, tmp->value);
    TEST_ASSERT_NULL(tmp->inf);
    TEST_ASSERT_NULL(tmp->sup);

    tmp = tmp->eq;
    TEST_ASSERT_NOT_NULL(tmp);
    TEST_ASSERT_EQUAL_CHAR('o', tmp->label);
    TEST_ASSERT_EQUAL(VALFIN, tmp->value);
    TEST_ASSERT_NULL(tmp->inf);
    TEST_ASSERT_NULL(tmp->eq);
    TEST_ASSERT_NULL(tmp->sup);

    deleteTH(&th);
}

/* White box test
 *
 * case #4:
 * full insertion,
 * relation to existing key only on first letter,
 * th->sup side insertion
 */
void test_f_ajoutTH_4(void)
{
    const char *cle1 = "hello";
    const char *cle2 = "hip";

    TrieHybride *th = newTH();
    th = ajoutTH(th, cle1, VALFIN);
    th = ajoutTH(th, cle2, VALFIN);

    TrieHybride *tmp = th;
    TEST_ASSERT_NOT_NULL(tmp);
    TEST_ASSERT_EQUAL_CHAR('h', tmp->label);
    TEST_ASSERT_EQUAL(VALVIDE, tmp->value);
    TEST_ASSERT_NULL(tmp->inf);
    TEST_ASSERT_NULL(tmp->sup);

    tmp = tmp->eq;
    TEST_ASSERT_NOT_NULL(tmp);
    TEST_ASSERT_EQUAL_CHAR('e', tmp->label);
    TEST_ASSERT_EQUAL(VALVIDE, tmp->value);
    TEST_ASSERT_NULL(tmp->inf);
    TEST_ASSERT_NOT_NULL(tmp->eq);

    tmp = tmp->sup;
    TEST_ASSERT_NOT_NULL(tmp);
    TEST_ASSERT_EQUAL_CHAR('i', tmp->label);
    TEST_ASSERT_EQUAL(VALVIDE, tmp->value);
    TEST_ASSERT_NULL(tmp->inf);
    TEST_ASSERT_NULL(tmp->sup);

    tmp = tmp->eq;
    TEST_ASSERT_NOT_NULL(tmp);
    TEST_ASSERT_EQUAL_CHAR('p', tmp->label);
    TEST_ASSERT_EQUAL(VALFIN, tmp->value);
    TEST_ASSERT_NULL(tmp->inf);
    TEST_ASSERT_NULL(tmp->eq);
    TEST_ASSERT_NULL(tmp->sup);

    deleteTH(&th);
}

/* White box test
 *
 * case #5:
 * full insertion,
 * relation to existing key only on first letter,
 * th->inf side insertion
 */
void test_f_ajoutTH_5(void)
{
    const char *cle1 = "hello";
    const char *cle2 = "hat";

    TrieHybride *th = newTH();
    th = ajoutTH(th, cle1, VALFIN);
    th = ajoutTH(th, cle2, VALFIN);

    TrieHybride *tmp = th;
    TEST_ASSERT_NOT_NULL(tmp);
    TEST_ASSERT_EQUAL_CHAR('h', tmp->label);
    TEST_ASSERT_EQUAL(VALVIDE, tmp->value);
    TEST_ASSERT_NULL(tmp->inf);
    TEST_ASSERT_NULL(tmp->sup);

    tmp = tmp->eq;
    TEST_ASSERT_NOT_NULL(tmp);
    TEST_ASSERT_EQUAL_CHAR('e', tmp->label);
    TEST_ASSERT_EQUAL(VALVIDE, tmp->value);
    TEST_ASSERT_NULL(tmp->sup);
    TEST_ASSERT_NOT_NULL(tmp->eq);

    tmp = tmp->inf;
    TEST_ASSERT_NOT_NULL(tmp);
    TEST_ASSERT_EQUAL_CHAR('a', tmp->label);
    TEST_ASSERT_EQUAL(VALVIDE, tmp->value);
    TEST_ASSERT_NULL(tmp->inf);
    TEST_ASSERT_NULL(tmp->sup);

    tmp = tmp->eq;
    TEST_ASSERT_NOT_NULL(tmp);
    TEST_ASSERT_EQUAL_CHAR('t', tmp->label);
    TEST_ASSERT_EQUAL(VALFIN, tmp->value);
    TEST_ASSERT_NULL(tmp->inf);
    TEST_ASSERT_NULL(tmp->eq);
    TEST_ASSERT_NULL(tmp->sup);

    deleteTH(&th);
}

/* White box test
 *
 * case #6:
 * full insertion,
 * relation to existing key only on first and second letters,
 * th->sup side insertion
 */
void test_f_ajoutTH_6(void)
{
    const char *cle1 = "hello";
    const char *cle2 = "herd";

    TrieHybride *th = newTH();
    th = ajoutTH(th, cle1, VALFIN);
    th = ajoutTH(th, cle2, VALFIN);

    TrieHybride *tmp = th;
    TEST_ASSERT_NOT_NULL(tmp);
    TEST_ASSERT_EQUAL_CHAR('h', tmp->label);
    TEST_ASSERT_EQUAL(VALVIDE, tmp->value);
    TEST_ASSERT_NULL(tmp->inf);
    TEST_ASSERT_NULL(tmp->sup);

    tmp = tmp->eq;
    TEST_ASSERT_NOT_NULL(tmp);
    TEST_ASSERT_EQUAL_CHAR('e', tmp->label);
    TEST_ASSERT_EQUAL(VALVIDE, tmp->value);
    TEST_ASSERT_NULL(tmp->inf);
    TEST_ASSERT_NULL(tmp->sup);

    tmp = tmp->eq;
    TEST_ASSERT_NOT_NULL(tmp);
    TEST_ASSERT_EQUAL_CHAR('l', tmp->label);
    TEST_ASSERT_EQUAL(VALVIDE, tmp->value);
    TEST_ASSERT_NULL(tmp->inf);
    TEST_ASSERT_NOT_NULL(tmp->eq);

    tmp = tmp->sup;
    TEST_ASSERT_NOT_NULL(tmp);
    TEST_ASSERT_EQUAL_CHAR('r', tmp->label);
    TEST_ASSERT_EQUAL(VALVIDE, tmp->value);
    TEST_ASSERT_NULL(tmp->inf);
    TEST_ASSERT_NULL(tmp->sup);

    tmp = tmp->eq;
    TEST_ASSERT_NOT_NULL(tmp);
    TEST_ASSERT_EQUAL_CHAR('d', tmp->label);
    TEST_ASSERT_EQUAL(VALFIN, tmp->value);
    TEST_ASSERT_NULL(tmp->inf);
    TEST_ASSERT_NULL(tmp->eq);
    TEST_ASSERT_NULL(tmp->sup);

    deleteTH(&th);
}

/* White box test
 *
 * case #7:
 * full insertion,
 * relation to existing key only on first and second letters,
 * th->inf side insertion
 */
void test_f_ajoutTH_7(void)
{
    const char *cle1 = "hello";
    const char *cle2 = "heft";

    TrieHybride *th = newTH();
    th = ajoutTH(th, cle1, VALFIN);
    th = ajoutTH(th, cle2, VALFIN);

    TrieHybride *tmp = th;
    TEST_ASSERT_NOT_NULL(tmp);
    TEST_ASSERT_EQUAL_CHAR('h', tmp->label);
    TEST_ASSERT_EQUAL(VALVIDE, tmp->value);
    TEST_ASSERT_NULL(tmp->inf);
    TEST_ASSERT_NULL(tmp->sup);

    tmp = tmp->eq;
    TEST_ASSERT_NOT_NULL(tmp);
    TEST_ASSERT_EQUAL_CHAR('e', tmp->label);
    TEST_ASSERT_EQUAL(VALVIDE, tmp->value);
    TEST_ASSERT_NULL(tmp->inf);
    TEST_ASSERT_NULL(tmp->sup);

    tmp = tmp->eq;
    TEST_ASSERT_NOT_NULL(tmp);
    TEST_ASSERT_EQUAL_CHAR('l', tmp->label);
    TEST_ASSERT_EQUAL(VALVIDE, tmp->value);
    TEST_ASSERT_NOT_NULL(tmp->eq);
    TEST_ASSERT_NULL(tmp->sup);

    tmp = tmp->inf;
    TEST_ASSERT_NOT_NULL(tmp);
    TEST_ASSERT_EQUAL_CHAR('f', tmp->label);
    TEST_ASSERT_EQUAL(VALVIDE, tmp->value);
    TEST_ASSERT_NULL(tmp->inf);
    TEST_ASSERT_NULL(tmp->sup);

    tmp = tmp->eq;
    TEST_ASSERT_NOT_NULL(tmp);
    TEST_ASSERT_EQUAL_CHAR('t', tmp->label);
    TEST_ASSERT_EQUAL(VALFIN, tmp->value);
    TEST_ASSERT_NULL(tmp->inf);
    TEST_ASSERT_NULL(tmp->eq);
    TEST_ASSERT_NULL(tmp->sup);

    deleteTH(&th);
}

/* White box test
 *
 * case #8:
 * full insertion,
 * all letters already stored but key not officially inserted
 */
void test_f_ajoutTH_8(void)
{
    const char *cle1 = "hello";
    const char *cle2 = "hell";

    TrieHybride *th = newTH();
    th = ajoutTH(th, cle1, VALFIN);
    th = ajoutTH(th, cle2, VALFIN);

    TrieHybride *tmp = th;
    TEST_ASSERT_NOT_NULL(tmp);
    TEST_ASSERT_EQUAL_CHAR('h', tmp->label);
    TEST_ASSERT_EQUAL(VALVIDE, tmp->value);
    TEST_ASSERT_NULL(tmp->inf);
    TEST_ASSERT_NULL(tmp->sup);

    tmp = tmp->eq;
    TEST_ASSERT_NOT_NULL(tmp);
    TEST_ASSERT_EQUAL_CHAR('e', tmp->label);
    TEST_ASSERT_EQUAL(VALVIDE, tmp->value);
    TEST_ASSERT_NULL(tmp->inf);
    TEST_ASSERT_NULL(tmp->sup);

    tmp = tmp->eq;
    TEST_ASSERT_NOT_NULL(tmp);
    TEST_ASSERT_EQUAL_CHAR('l', tmp->label);
    TEST_ASSERT_EQUAL(VALVIDE, tmp->value);
    TEST_ASSERT_NULL(tmp->inf);
    TEST_ASSERT_NULL(tmp->sup);

    tmp = tmp->eq;
    TEST_ASSERT_NOT_NULL(tmp);
    TEST_ASSERT_EQUAL_CHAR('l', tmp->label);
    TEST_ASSERT_EQUAL(VALFIN, tmp->value);
    TEST_ASSERT_NULL(tmp->inf);
    TEST_ASSERT_NOT_NULL(tmp->eq);
    TEST_ASSERT_NULL(tmp->sup);

    deleteTH(&th);
}

/* White box test
 *
 * case #9:
 * full insertion,
 * key already inserted
 */
void test_f_ajoutTH_9(void)
{
    const char *cle = "hell";

    TrieHybride *th = newTH();
    th = ajoutTH(th, cle, 1);
    th = ajoutTH(th, cle, 2);

    TrieHybride *tmp = th;
    TEST_ASSERT_NOT_NULL(tmp);
    TEST_ASSERT_EQUAL_CHAR('h', tmp->label);
    TEST_ASSERT_EQUAL(VALVIDE, tmp->value);
    TEST_ASSERT_NULL(tmp->inf);
    TEST_ASSERT_NULL(tmp->sup);

    tmp = tmp->eq;
    TEST_ASSERT_NOT_NULL(tmp);
    TEST_ASSERT_EQUAL_CHAR('e', tmp->label);
    TEST_ASSERT_EQUAL(VALVIDE, tmp->value);
    TEST_ASSERT_NULL(tmp->inf);
    TEST_ASSERT_NULL(tmp->sup);

    tmp = tmp->eq;
    TEST_ASSERT_NOT_NULL(tmp);
    TEST_ASSERT_EQUAL_CHAR('l', tmp->label);
    TEST_ASSERT_EQUAL(VALVIDE, tmp->value);
    TEST_ASSERT_NULL(tmp->inf);
    TEST_ASSERT_NULL(tmp->sup);

    tmp = tmp->eq;
    TEST_ASSERT_NOT_NULL(tmp);
    TEST_ASSERT_EQUAL_CHAR('l', tmp->label);
    TEST_ASSERT_EQUAL(1, tmp->value);
    TEST_ASSERT_NULL(tmp->inf);
    TEST_ASSERT_NULL(tmp->eq);
    TEST_ASSERT_NULL(tmp->sup);

    deleteTH(&th);
}

/* White box test
 *
 * case #1:
 * delete on empty trie
 */
void test_f_supprTH_1(void)
{
    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);
    th = supprTH(th, "hello");
    TEST_ASSERT_NULL(th);
}

/* White box test
 *
 * case #2:
 * delete existing key
 */
void test_f_supprTH_2(void)
{
    const char *cle = "hip";

    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);

    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);
    TEST_ASSERT_NOT_NULL(th->eq);
    TEST_ASSERT_NOT_NULL(th->eq->eq);
    TEST_ASSERT_EQUAL_CHAR('p', th->eq->eq->label);
    TEST_ASSERT(th->eq->eq->value == VALFIN);
    TEST_ASSERT_NULL(th->eq->eq->eq);

    th = supprTH(th, cle);
    TEST_ASSERT_NULL(th);
}

/* White box test
 *
 * case #3:
 * delete existing key that shares a prefix with another existing key
 */
void test_f_supprTH_3(void)
{
    const char *cle;

    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);

    cle = "prop";
    th = ajoutTH(th, cle, VALFIN);
    cle = "pro";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);
    TEST_ASSERT_NOT_NULL(th->eq);
    TEST_ASSERT_NOT_NULL(th->eq->eq);
    TEST_ASSERT_EQUAL_CHAR('o', th->eq->eq->label);
    TEST_ASSERT(th->eq->eq->value == VALFIN);
    TEST_ASSERT_NOT_NULL(th->eq->eq->eq);
    TEST_ASSERT_EQUAL_CHAR('p', th->eq->eq->eq->label);
    TEST_ASSERT(th->eq->eq->eq->value == VALFIN);
    TEST_ASSERT_NULL(th->eq->eq->eq->eq);

    cle = "prop";
    th = supprTH(th, cle);
    TEST_ASSERT_NOT_NULL(th);
    TEST_ASSERT_NOT_NULL(th->eq);
    TEST_ASSERT_NOT_NULL(th->eq->eq);
    TEST_ASSERT_EQUAL_CHAR('o', th->eq->eq->label);
    TEST_ASSERT(th->eq->eq->value == VALFIN);
    TEST_ASSERT_NULL(th->eq->eq->eq);

    deleteTH(&th);
    TEST_ASSERT_NULL(th);
}

/* White box test
 *
 * case #4:
 * delete existing key that is a prefix of another existing key
 */
void test_f_supprTH_4(void)
{
    const char *cle;

    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);

    cle = "prop";
    th = ajoutTH(th, cle, VALFIN);
    cle = "pro";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);
    TEST_ASSERT_NOT_NULL(th->eq);
    TEST_ASSERT_NOT_NULL(th->eq->eq);
    TEST_ASSERT_EQUAL_CHAR('o', th->eq->eq->label);
    TEST_ASSERT(th->eq->eq->value == VALFIN);
    TEST_ASSERT_NOT_NULL(th->eq->eq->eq);
    TEST_ASSERT_EQUAL_CHAR('p', th->eq->eq->eq->label);
    TEST_ASSERT(th->eq->eq->eq->value == VALFIN);
    TEST_ASSERT_NULL(th->eq->eq->eq->eq);

    cle = "pro";
    th = supprTH(th, cle);
    TEST_ASSERT_NOT_NULL(th);
    TEST_ASSERT_NOT_NULL(th->eq);
    TEST_ASSERT_NOT_NULL(th->eq->eq);
    TEST_ASSERT_EQUAL_CHAR('o', th->eq->eq->label);
    TEST_ASSERT(th->eq->eq->value == VALVIDE);
    TEST_ASSERT_NOT_NULL(th->eq->eq->eq);
    TEST_ASSERT_EQUAL_CHAR('p', th->eq->eq->eq->label);
    TEST_ASSERT(th->eq->eq->eq->value == VALFIN);
    TEST_ASSERT_NULL(th->eq->eq->eq->eq);

    deleteTH(&th);
    TEST_ASSERT_NULL(th);
}

/* White box test
 *
 * case #5:
 * delete existing key that is a prefix of another existing key
 */
void test_f_supprTH_5(void)
{
    const char *cle;

    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);

    cle = "prop";
    th = ajoutTH(th, cle, VALFIN);
    cle = "pr";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);
    TEST_ASSERT_NOT_NULL(th->eq);
    TEST_ASSERT_EQUAL_CHAR('r', th->eq->label);
    TEST_ASSERT(th->eq->value == VALFIN);
    TEST_ASSERT_NOT_NULL(th->eq->eq);
    TEST_ASSERT_NOT_NULL(th->eq->eq->eq);
    TEST_ASSERT_EQUAL_CHAR('p', th->eq->eq->eq->label);
    TEST_ASSERT(th->eq->eq->eq->value == VALFIN);
    TEST_ASSERT_NULL(th->eq->eq->eq->eq);

    cle = "pr";
    th = supprTH(th, cle);
    TEST_ASSERT_NOT_NULL(th);
    TEST_ASSERT_NOT_NULL(th->eq);
    TEST_ASSERT_EQUAL_CHAR('r', th->eq->label);
    TEST_ASSERT(th->eq->value == VALVIDE);
    TEST_ASSERT_NOT_NULL(th->eq->eq);
    TEST_ASSERT_NOT_NULL(th->eq->eq->eq);
    TEST_ASSERT_EQUAL_CHAR('p', th->eq->eq->eq->label);
    TEST_ASSERT(th->eq->eq->eq->value == VALFIN);
    TEST_ASSERT_NULL(th->eq->eq->eq->eq);

    deleteTH(&th);
    TEST_ASSERT_NULL(th);
}

/* White box test
 *
 * case #6:
 * delete inexistant key with no relation to any present keys of a populated trie
 */
void test_f_supprTH_6(void)
{
    const char *cle;

    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);

    cle = "pro";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);
    TEST_ASSERT_NOT_NULL(th->eq);
    TEST_ASSERT_NOT_NULL(th->eq->eq);
    TEST_ASSERT_EQUAL_CHAR('o', th->eq->eq->label);
    TEST_ASSERT(th->eq->eq->value == VALFIN);
    TEST_ASSERT_NULL(th->eq->eq->eq);

    cle = "fly";
    th = supprTH(th, cle);
    TEST_ASSERT_NOT_NULL(th);
    TEST_ASSERT_NOT_NULL(th->eq);
    TEST_ASSERT_NOT_NULL(th->eq->eq);
    TEST_ASSERT_EQUAL_CHAR('o', th->eq->eq->label);
    TEST_ASSERT(th->eq->eq->value == VALFIN);
    TEST_ASSERT_NULL(th->eq->eq->eq);

    deleteTH(&th);
    TEST_ASSERT_NULL(th);
}

/* White box test
 *
 * case #7:
 * delete inexistant key that shares a prefix with a key of a populated trie
 */
void test_f_supprTH_7(void)
{
    const char *cle;

    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);

    cle = "pro";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);
    TEST_ASSERT_NOT_NULL(th->eq);
    TEST_ASSERT_NOT_NULL(th->eq->eq);
    TEST_ASSERT_EQUAL_CHAR('o', th->eq->eq->label);
    TEST_ASSERT(th->eq->eq->value == VALFIN);
    TEST_ASSERT_NULL(th->eq->eq->eq);

    cle = "prelude";
    th = supprTH(th, cle);
    TEST_ASSERT_NOT_NULL(th);
    TEST_ASSERT_NOT_NULL(th->eq);
    TEST_ASSERT_NOT_NULL(th->eq->eq);
    TEST_ASSERT_EQUAL_CHAR('o', th->eq->eq->label);
    TEST_ASSERT(th->eq->eq->value == VALFIN);
    TEST_ASSERT_NULL(th->eq->eq->eq);

    deleteTH(&th);
    TEST_ASSERT_NULL(th);
}

/* White box test
 *
 * case #8:
 * reparenting variation,
 * reparent inf child,
 * inf child is a terminal node with no children
 */
void test_f_supprTH_8(void)
{
    const char *cle;

    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);

    cle = "her";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "ha";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "her";
    th = supprTH(th, cle);
    TEST_ASSERT_NOT_NULL(th);
    TEST_ASSERT_NULL(th->inf);
    TEST_ASSERT_NULL(th->sup);
    TEST_ASSERT_EQUAL_CHAR('h', th->label);
    TEST_ASSERT(th->value == VALVIDE);
    TEST_ASSERT_NOT_NULL(th->eq);
    TEST_ASSERT_NULL(th->eq->inf);
    TEST_ASSERT_NULL(th->eq->eq);
    TEST_ASSERT_NULL(th->eq->sup);
    TEST_ASSERT_EQUAL_CHAR('a', th->eq->label);
    TEST_ASSERT(th->eq->value == VALFIN);

    deleteTH(&th);
    TEST_ASSERT_NULL(th);
}

/* White box test
 *
 * case #9:
 * reparenting variation,
 * reparent inf child,
 * inf child is not a terminal node
 */
void test_f_supprTH_9(void)
{
    const char *cle;

    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);

    cle = "her";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "hat";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "her";
    th = supprTH(th, cle);
    TEST_ASSERT_NOT_NULL(th);
    TEST_ASSERT_NULL(th->inf);
    TEST_ASSERT_NULL(th->sup);
    TEST_ASSERT_EQUAL_CHAR('h', th->label);
    TEST_ASSERT(th->value == VALVIDE);
    TEST_ASSERT_NOT_NULL(th->eq);
    TEST_ASSERT_NULL(th->eq->inf);
    TEST_ASSERT_NULL(th->eq->sup);
    TEST_ASSERT_EQUAL_CHAR('a', th->eq->label);
    TEST_ASSERT(th->eq->value == VALVIDE);
    TEST_ASSERT_NOT_NULL(th->eq->eq);
    TEST_ASSERT_NULL(th->eq->eq->inf);
    TEST_ASSERT_NULL(th->eq->eq->eq);
    TEST_ASSERT_NULL(th->eq->eq->sup);
    TEST_ASSERT_EQUAL_CHAR('t', th->eq->eq->label);
    TEST_ASSERT(th->eq->eq->value == VALFIN);

    deleteTH(&th);
    TEST_ASSERT_NULL(th);
}

/* White box test
 *
 * case #10:
 * reparenting variation,
 * reparent inf child,
 * inf child is a terminal node with only another inf child
 */
void test_f_supprTH_10(void)
{
    const char *cle;

    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);

    cle = "her";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "hd";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "ha";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "hd";
    th = supprTH(th, cle);
    TEST_ASSERT_NOT_NULL(th);
    TEST_ASSERT_NULL(th->inf);
    TEST_ASSERT_NULL(th->sup);
    TEST_ASSERT_EQUAL_CHAR('h', th->label);
    TEST_ASSERT(th->value == VALVIDE);
    TEST_ASSERT_NOT_NULL(th->eq);
    TEST_ASSERT_NOT_NULL(th->eq->inf);
    TEST_ASSERT_NULL(th->eq->sup);
    TEST_ASSERT_EQUAL_CHAR('e', th->eq->label);
    TEST_ASSERT(th->eq->value == VALVIDE);
    TEST_ASSERT_EQUAL_CHAR('a', th->eq->inf->label);
    TEST_ASSERT(th->eq->inf->value == VALFIN);
    TEST_ASSERT_NULL(th->eq->inf->inf);
    TEST_ASSERT_NULL(th->eq->inf->eq);
    TEST_ASSERT_NULL(th->eq->inf->sup);
    TEST_ASSERT_NOT_NULL(th->eq->eq);
    TEST_ASSERT_NULL(th->eq->eq->inf);
    TEST_ASSERT_NULL(th->eq->eq->eq);
    TEST_ASSERT_NULL(th->eq->eq->sup);
    TEST_ASSERT_EQUAL_CHAR('r', th->eq->eq->label);
    TEST_ASSERT(th->eq->eq->value == VALFIN);

    deleteTH(&th);
    TEST_ASSERT_NULL(th);
}

/* White box test
 *
 * case #11:
 * reparenting variation,
 * reparent inf child,
 * inf child is a terminal node with only another sup child
 */
void test_f_supprTH_11(void)
{
    const char *cle;

    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);

    cle = "her";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "ha";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "hd";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "ha";
    th = supprTH(th, cle);
    TEST_ASSERT_NOT_NULL(th);
    TEST_ASSERT_NULL(th->inf);
    TEST_ASSERT_NULL(th->sup);
    TEST_ASSERT_EQUAL_CHAR('h', th->label);
    TEST_ASSERT(th->value == VALVIDE);
    TEST_ASSERT_NOT_NULL(th->eq);
    TEST_ASSERT_NOT_NULL(th->eq->inf);
    TEST_ASSERT_NULL(th->eq->sup);
    TEST_ASSERT_EQUAL_CHAR('e', th->eq->label);
    TEST_ASSERT(th->eq->value == VALVIDE);
    TEST_ASSERT_EQUAL_CHAR('d', th->eq->inf->label);
    TEST_ASSERT(th->eq->inf->value == VALFIN);
    TEST_ASSERT_NULL(th->eq->inf->inf);
    TEST_ASSERT_NULL(th->eq->inf->eq);
    TEST_ASSERT_NULL(th->eq->inf->sup);
    TEST_ASSERT_NOT_NULL(th->eq->eq);
    TEST_ASSERT_NULL(th->eq->eq->inf);
    TEST_ASSERT_NULL(th->eq->eq->eq);
    TEST_ASSERT_NULL(th->eq->eq->sup);
    TEST_ASSERT_EQUAL_CHAR('r', th->eq->eq->label);
    TEST_ASSERT(th->eq->eq->value == VALFIN);

    deleteTH(&th);
    TEST_ASSERT_NULL(th);
}

/* White box test
 *
 * case #12:
 * reparenting variation,
 * reparent inf child,
 * inf child is a terminal node with both inf and sup children
 */
void test_f_supprTH_12(void)
{
    const char *cle;

    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);

    cle = "her";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "hb";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "ha";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "hd";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "hb";
    th = supprTH(th, cle);
    TEST_ASSERT_NOT_NULL(th);
    TEST_ASSERT_NULL(th->inf);
    TEST_ASSERT_NULL(th->sup);
    TEST_ASSERT_EQUAL_CHAR('h', th->label);
    TEST_ASSERT(th->value == VALVIDE);
    TEST_ASSERT_NOT_NULL(th->eq);
    TEST_ASSERT_NOT_NULL(th->eq->inf);
    TEST_ASSERT_NULL(th->eq->sup);
    TEST_ASSERT_EQUAL_CHAR('e', th->eq->label);
    TEST_ASSERT(th->eq->value == VALVIDE);
    TEST_ASSERT_EQUAL_CHAR('a', th->eq->inf->label);
    TEST_ASSERT(th->eq->inf->value == VALFIN);
    TEST_ASSERT_NULL(th->eq->inf->inf);
    TEST_ASSERT_NULL(th->eq->inf->eq);
    TEST_ASSERT_NOT_NULL(th->eq->inf->sup);
    TEST_ASSERT_EQUAL_CHAR('d', th->eq->inf->sup->label);
    TEST_ASSERT(th->eq->inf->sup->value == VALFIN);
    TEST_ASSERT_NULL(th->eq->inf->sup->inf);
    TEST_ASSERT_NULL(th->eq->inf->sup->eq);
    TEST_ASSERT_NULL(th->eq->inf->sup->sup);
    TEST_ASSERT_NOT_NULL(th->eq->eq);
    TEST_ASSERT_NULL(th->eq->eq->inf);
    TEST_ASSERT_NULL(th->eq->eq->eq);
    TEST_ASSERT_NULL(th->eq->eq->sup);
    TEST_ASSERT_EQUAL_CHAR('r', th->eq->eq->label);
    TEST_ASSERT(th->eq->eq->value == VALFIN);

    deleteTH(&th);
    TEST_ASSERT_NULL(th);
}

/* White box test
 *
 * case #13:
 * reparenting variation,
 * reparent inf child,
 * inf child is not a terminal node with both inf and sup children
 */
void test_f_supprTH_13(void)
{
    const char *cle;

    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);

    cle = "her";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "hbo";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "ha";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "hd";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "hbo";
    th = supprTH(th, cle);
    TEST_ASSERT_NOT_NULL(th);
    TEST_ASSERT_NULL(th->inf);
    TEST_ASSERT_NULL(th->sup);
    TEST_ASSERT_EQUAL_CHAR('h', th->label);
    TEST_ASSERT(th->value == VALVIDE);
    TEST_ASSERT_NOT_NULL(th->eq);
    TEST_ASSERT_NOT_NULL(th->eq->inf);
    TEST_ASSERT_NULL(th->eq->sup);
    TEST_ASSERT_EQUAL_CHAR('e', th->eq->label);
    TEST_ASSERT(th->eq->value == VALVIDE);
    TEST_ASSERT_EQUAL_CHAR('a', th->eq->inf->label);
    TEST_ASSERT(th->eq->inf->value == VALFIN);
    TEST_ASSERT_NULL(th->eq->inf->inf);
    TEST_ASSERT_NULL(th->eq->inf->eq);
    TEST_ASSERT_NOT_NULL(th->eq->inf->sup);
    TEST_ASSERT_EQUAL_CHAR('d', th->eq->inf->sup->label);
    TEST_ASSERT(th->eq->inf->sup->value == VALFIN);
    TEST_ASSERT_NULL(th->eq->inf->sup->inf);
    TEST_ASSERT_NULL(th->eq->inf->sup->eq);
    TEST_ASSERT_NULL(th->eq->inf->sup->sup);
    TEST_ASSERT_NOT_NULL(th->eq->eq);
    TEST_ASSERT_NULL(th->eq->eq->inf);
    TEST_ASSERT_NULL(th->eq->eq->eq);
    TEST_ASSERT_NULL(th->eq->eq->sup);
    TEST_ASSERT_EQUAL_CHAR('r', th->eq->eq->label);
    TEST_ASSERT(th->eq->eq->value == VALFIN);

    deleteTH(&th);
    TEST_ASSERT_NULL(th);
}

/* White box test
 *
 * case #14:
 * reparenting variation,
 * reparent inf child,
 * inf child is not a terminal node with only an inf child
 */
void test_f_supprTH_14(void)
{
    const char *cle;

    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);

    cle = "her";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "hbo";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "ha";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "hbo";
    th = supprTH(th, cle);
    TEST_ASSERT_NOT_NULL(th);
    TEST_ASSERT_NULL(th->inf);
    TEST_ASSERT_NULL(th->sup);
    TEST_ASSERT_EQUAL_CHAR('h', th->label);
    TEST_ASSERT(th->value == VALVIDE);
    TEST_ASSERT_NOT_NULL(th->eq);
    TEST_ASSERT_NOT_NULL(th->eq->inf);
    TEST_ASSERT_NULL(th->eq->sup);
    TEST_ASSERT_EQUAL_CHAR('e', th->eq->label);
    TEST_ASSERT(th->eq->value == VALVIDE);
    TEST_ASSERT_EQUAL_CHAR('a', th->eq->inf->label);
    TEST_ASSERT(th->eq->inf->value == VALFIN);
    TEST_ASSERT_NULL(th->eq->inf->inf);
    TEST_ASSERT_NULL(th->eq->inf->eq);
    TEST_ASSERT_NULL(th->eq->inf->sup);
    TEST_ASSERT_NOT_NULL(th->eq->eq);
    TEST_ASSERT_NULL(th->eq->eq->inf);
    TEST_ASSERT_NULL(th->eq->eq->eq);
    TEST_ASSERT_NULL(th->eq->eq->sup);
    TEST_ASSERT_EQUAL_CHAR('r', th->eq->eq->label);
    TEST_ASSERT(th->eq->eq->value == VALFIN);

    deleteTH(&th);
    TEST_ASSERT_NULL(th);
}

/* White box test
 *
 * case #15:
 * reparenting variation,
 * reparent inf child,
 * inf child is not a terminal node with only a sup child
 */
void test_f_supprTH_15(void)
{
    const char *cle;

    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);

    cle = "her";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "hbo";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "hd";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "hbo";
    th = supprTH(th, cle);
    TEST_ASSERT_NOT_NULL(th);
    TEST_ASSERT_NULL(th->inf);
    TEST_ASSERT_NULL(th->sup);
    TEST_ASSERT_EQUAL_CHAR('h', th->label);
    TEST_ASSERT(th->value == VALVIDE);
    TEST_ASSERT_NOT_NULL(th->eq);
    TEST_ASSERT_NOT_NULL(th->eq->inf);
    TEST_ASSERT_NULL(th->eq->sup);
    TEST_ASSERT_EQUAL_CHAR('e', th->eq->label);
    TEST_ASSERT(th->eq->value == VALVIDE);
    TEST_ASSERT_EQUAL_CHAR('d', th->eq->inf->label);
    TEST_ASSERT(th->eq->inf->value == VALFIN);
    TEST_ASSERT_NULL(th->eq->inf->inf);
    TEST_ASSERT_NULL(th->eq->inf->eq);
    TEST_ASSERT_NULL(th->eq->inf->sup);
    TEST_ASSERT_NOT_NULL(th->eq->eq);
    TEST_ASSERT_NULL(th->eq->eq->inf);
    TEST_ASSERT_NULL(th->eq->eq->eq);
    TEST_ASSERT_NULL(th->eq->eq->sup);
    TEST_ASSERT_EQUAL_CHAR('r', th->eq->eq->label);
    TEST_ASSERT(th->eq->eq->value == VALFIN);

    deleteTH(&th);
    TEST_ASSERT_NULL(th);
}

/* White box test
 *
 * case #16:
 * reparenting variation,
 * reparent sup child,
 * sup child is a terminal node with no children
 */
void test_f_supprTH_16(void)
{
    const char *cle;

    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);

    cle = "her";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "hi";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "her";
    th = supprTH(th, cle);
    TEST_ASSERT_NOT_NULL(th);
    TEST_ASSERT_NULL(th->inf);
    TEST_ASSERT_NULL(th->sup);
    TEST_ASSERT_EQUAL_CHAR('h', th->label);
    TEST_ASSERT(th->value == VALVIDE);
    TEST_ASSERT_NOT_NULL(th->eq);
    TEST_ASSERT_NULL(th->eq->inf);
    TEST_ASSERT_NULL(th->eq->eq);
    TEST_ASSERT_NULL(th->eq->sup);
    TEST_ASSERT_EQUAL_CHAR('i', th->eq->label);
    TEST_ASSERT(th->eq->value == VALFIN);

    deleteTH(&th);
    TEST_ASSERT_NULL(th);
}

/* White box test
 *
 * case #17:
 * reparenting variation,
 * reparent sup child,
 * sup child is not a terminal node
 */
void test_f_supprTH_17(void)
{
    const char *cle;

    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);

    cle = "her";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "hit";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "her";
    th = supprTH(th, cle);
    TEST_ASSERT_NOT_NULL(th);
    TEST_ASSERT_NULL(th->inf);
    TEST_ASSERT_NULL(th->sup);
    TEST_ASSERT_EQUAL_CHAR('h', th->label);
    TEST_ASSERT(th->value == VALVIDE);
    TEST_ASSERT_NOT_NULL(th->eq);
    TEST_ASSERT_NULL(th->eq->inf);
    TEST_ASSERT_NULL(th->eq->sup);
    TEST_ASSERT_EQUAL_CHAR('i', th->eq->label);
    TEST_ASSERT(th->eq->value == VALVIDE);
    TEST_ASSERT_NOT_NULL(th->eq->eq);
    TEST_ASSERT_NULL(th->eq->eq->inf);
    TEST_ASSERT_NULL(th->eq->eq->eq);
    TEST_ASSERT_NULL(th->eq->eq->sup);
    TEST_ASSERT_EQUAL_CHAR('t', th->eq->eq->label);
    TEST_ASSERT(th->eq->eq->value == VALFIN);

    deleteTH(&th);
    TEST_ASSERT_NULL(th);
}

/* White box test
 *
 * case #18:
 * reparenting variation,
 * reparent sup child,
 * sup child is a terminal node with only another inf child
 */
void test_f_supprTH_18(void)
{
    const char *cle;

    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);

    cle = "her";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "hi";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "hg";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "hi";
    th = supprTH(th, cle);
    TEST_ASSERT_NOT_NULL(th);
    TEST_ASSERT_NULL(th->inf);
    TEST_ASSERT_NULL(th->sup);
    TEST_ASSERT_EQUAL_CHAR('h', th->label);
    TEST_ASSERT(th->value == VALVIDE);
    TEST_ASSERT_NOT_NULL(th->eq);
    TEST_ASSERT_NULL(th->eq->inf);
    TEST_ASSERT_NOT_NULL(th->eq->sup);
    TEST_ASSERT_EQUAL_CHAR('e', th->eq->label);
    TEST_ASSERT(th->eq->value == VALVIDE);
    TEST_ASSERT_EQUAL_CHAR('g', th->eq->sup->label);
    TEST_ASSERT(th->eq->sup->value == VALFIN);
    TEST_ASSERT_NULL(th->eq->sup->inf);
    TEST_ASSERT_NULL(th->eq->sup->eq);
    TEST_ASSERT_NULL(th->eq->sup->sup);
    TEST_ASSERT_NOT_NULL(th->eq->eq);
    TEST_ASSERT_NULL(th->eq->eq->inf);
    TEST_ASSERT_NULL(th->eq->eq->eq);
    TEST_ASSERT_NULL(th->eq->eq->sup);
    TEST_ASSERT_EQUAL_CHAR('r', th->eq->eq->label);
    TEST_ASSERT(th->eq->eq->value == VALFIN);

    deleteTH(&th);
    TEST_ASSERT_NULL(th);
}

/* White box test
 *
 * case #19:
 * reparenting variation,
 * reparent sup child,
 * sup child is a terminal node with only another sup child
 */
void test_f_supprTH_19(void)
{
    const char *cle;

    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);

    cle = "her";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "hi";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "ho";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "hi";
    th = supprTH(th, cle);
    TEST_ASSERT_NOT_NULL(th);
    TEST_ASSERT_NULL(th->inf);
    TEST_ASSERT_NULL(th->sup);
    TEST_ASSERT_EQUAL_CHAR('h', th->label);
    TEST_ASSERT(th->value == VALVIDE);
    TEST_ASSERT_NOT_NULL(th->eq);
    TEST_ASSERT_NULL(th->eq->inf);
    TEST_ASSERT_NOT_NULL(th->eq->sup);
    TEST_ASSERT_EQUAL_CHAR('e', th->eq->label);
    TEST_ASSERT(th->eq->value == VALVIDE);
    TEST_ASSERT_EQUAL_CHAR('o', th->eq->sup->label);
    TEST_ASSERT(th->eq->sup->value == VALFIN);
    TEST_ASSERT_NULL(th->eq->sup->inf);
    TEST_ASSERT_NULL(th->eq->sup->eq);
    TEST_ASSERT_NULL(th->eq->sup->sup);
    TEST_ASSERT_NOT_NULL(th->eq->eq);
    TEST_ASSERT_NULL(th->eq->eq->inf);
    TEST_ASSERT_NULL(th->eq->eq->eq);
    TEST_ASSERT_NULL(th->eq->eq->sup);
    TEST_ASSERT_EQUAL_CHAR('r', th->eq->eq->label);
    TEST_ASSERT(th->eq->eq->value == VALFIN);

    deleteTH(&th);
    TEST_ASSERT_NULL(th);
}

/* White box test
 *
 * case #20:
 * reparenting variation,
 * reparent sup child,
 * sup child is a terminal node with both inf and sup children
 */
void test_f_supprTH_20(void)
{
    const char *cle;

    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);

    cle = "her";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "hi";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "hg";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "ho";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "hi";
    th = supprTH(th, cle);
    TEST_ASSERT_NOT_NULL(th);
    TEST_ASSERT_NULL(th->inf);
    TEST_ASSERT_NULL(th->sup);
    TEST_ASSERT_EQUAL_CHAR('h', th->label);
    TEST_ASSERT(th->value == VALVIDE);
    TEST_ASSERT_NOT_NULL(th->eq);
    TEST_ASSERT_NULL(th->eq->inf);
    TEST_ASSERT_NOT_NULL(th->eq->sup);
    TEST_ASSERT_EQUAL_CHAR('e', th->eq->label);
    TEST_ASSERT(th->eq->value == VALVIDE);
    TEST_ASSERT_EQUAL_CHAR('g', th->eq->sup->label);
    TEST_ASSERT(th->eq->sup->value == VALFIN);
    TEST_ASSERT_NULL(th->eq->sup->inf);
    TEST_ASSERT_NULL(th->eq->sup->eq);
    TEST_ASSERT_NOT_NULL(th->eq->sup->sup);
    TEST_ASSERT_EQUAL_CHAR('o', th->eq->sup->sup->label);
    TEST_ASSERT(th->eq->sup->sup->value == VALFIN);
    TEST_ASSERT_NULL(th->eq->sup->sup->inf);
    TEST_ASSERT_NULL(th->eq->sup->sup->eq);
    TEST_ASSERT_NULL(th->eq->sup->sup->sup);
    TEST_ASSERT_NOT_NULL(th->eq->eq);
    TEST_ASSERT_NULL(th->eq->eq->inf);
    TEST_ASSERT_NULL(th->eq->eq->eq);
    TEST_ASSERT_NULL(th->eq->eq->sup);
    TEST_ASSERT_EQUAL_CHAR('r', th->eq->eq->label);
    TEST_ASSERT(th->eq->eq->value == VALFIN);

    deleteTH(&th);
    TEST_ASSERT_NULL(th);
}

/* White box test
 *
 * case #21:
 * reparenting variation,
 * reparent sup child,
 * sup child is not a terminal node with both inf and sup children
 */
void test_f_supprTH_21(void)
{
    const char *cle;

    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);

    cle = "her";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "hit";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "hg";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "ho";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "hit";
    th = supprTH(th, cle);
    TEST_ASSERT_NOT_NULL(th);
    TEST_ASSERT_NULL(th->inf);
    TEST_ASSERT_NULL(th->sup);
    TEST_ASSERT_EQUAL_CHAR('h', th->label);
    TEST_ASSERT(th->value == VALVIDE);
    TEST_ASSERT_NOT_NULL(th->eq);
    TEST_ASSERT_NULL(th->eq->inf);
    TEST_ASSERT_NOT_NULL(th->eq->sup);
    TEST_ASSERT_EQUAL_CHAR('e', th->eq->label);
    TEST_ASSERT(th->eq->value == VALVIDE);
    TEST_ASSERT_EQUAL_CHAR('g', th->eq->sup->label);
    TEST_ASSERT(th->eq->sup->value == VALFIN);
    TEST_ASSERT_NULL(th->eq->sup->inf);
    TEST_ASSERT_NULL(th->eq->sup->eq);
    TEST_ASSERT_NOT_NULL(th->eq->sup->sup);
    TEST_ASSERT_EQUAL_CHAR('o', th->eq->sup->sup->label);
    TEST_ASSERT(th->eq->sup->sup->value == VALFIN);
    TEST_ASSERT_NULL(th->eq->sup->sup->inf);
    TEST_ASSERT_NULL(th->eq->sup->sup->eq);
    TEST_ASSERT_NULL(th->eq->sup->sup->sup);
    TEST_ASSERT_NOT_NULL(th->eq->eq);
    TEST_ASSERT_NULL(th->eq->eq->inf);
    TEST_ASSERT_NULL(th->eq->eq->eq);
    TEST_ASSERT_NULL(th->eq->eq->sup);
    TEST_ASSERT_EQUAL_CHAR('r', th->eq->eq->label);
    TEST_ASSERT(th->eq->eq->value == VALFIN);

    deleteTH(&th);
    TEST_ASSERT_NULL(th);
}

/* White box test
 *
 * case #22:
 * reparenting variation,
 * reparent sup child,
 * sup child is not a terminal node with only an inf child
 */
void test_f_supprTH_22(void)
{
    const char *cle;

    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);

    cle = "her";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "hit";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "hg";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "hit";
    th = supprTH(th, cle);
    TEST_ASSERT_NOT_NULL(th);
    TEST_ASSERT_NULL(th->inf);
    TEST_ASSERT_NULL(th->sup);
    TEST_ASSERT_EQUAL_CHAR('h', th->label);
    TEST_ASSERT(th->value == VALVIDE);
    TEST_ASSERT_NOT_NULL(th->eq);
    TEST_ASSERT_NULL(th->eq->inf);
    TEST_ASSERT_NOT_NULL(th->eq->sup);
    TEST_ASSERT_EQUAL_CHAR('e', th->eq->label);
    TEST_ASSERT(th->eq->value == VALVIDE);
    TEST_ASSERT_EQUAL_CHAR('g', th->eq->sup->label);
    TEST_ASSERT(th->eq->sup->value == VALFIN);
    TEST_ASSERT_NULL(th->eq->sup->inf);
    TEST_ASSERT_NULL(th->eq->sup->eq);
    TEST_ASSERT_NULL(th->eq->sup->sup);
    TEST_ASSERT_NOT_NULL(th->eq->eq);
    TEST_ASSERT_NULL(th->eq->eq->inf);
    TEST_ASSERT_NULL(th->eq->eq->eq);
    TEST_ASSERT_NULL(th->eq->eq->sup);
    TEST_ASSERT_EQUAL_CHAR('r', th->eq->eq->label);
    TEST_ASSERT(th->eq->eq->value == VALFIN);

    deleteTH(&th);
    TEST_ASSERT_NULL(th);
}

/* White box test
 *
 * case #23:
 * reparenting variation,
 * reparent sup child,
 * sup child is not a terminal node with only a sup child
 */
void test_f_supprTH_23(void)
{
    const char *cle;

    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);

    cle = "her";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "hit";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "ho";
    th = ajoutTH(th, cle, VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    cle = "hit";
    th = supprTH(th, cle);
    TEST_ASSERT_NOT_NULL(th);
    TEST_ASSERT_NULL(th->inf);
    TEST_ASSERT_NULL(th->sup);
    TEST_ASSERT_EQUAL_CHAR('h', th->label);
    TEST_ASSERT(th->value == VALVIDE);
    TEST_ASSERT_NOT_NULL(th->eq);
    TEST_ASSERT_NULL(th->eq->inf);
    TEST_ASSERT_NOT_NULL(th->eq->sup);
    TEST_ASSERT_EQUAL_CHAR('e', th->eq->label);
    TEST_ASSERT(th->eq->value == VALVIDE);
    TEST_ASSERT_EQUAL_CHAR('o', th->eq->sup->label);
    TEST_ASSERT(th->eq->sup->value == VALFIN);
    TEST_ASSERT_NULL(th->eq->sup->inf);
    TEST_ASSERT_NULL(th->eq->sup->eq);
    TEST_ASSERT_NULL(th->eq->sup->sup);
    TEST_ASSERT_NOT_NULL(th->eq->eq);
    TEST_ASSERT_NULL(th->eq->eq->inf);
    TEST_ASSERT_NULL(th->eq->eq->eq);
    TEST_ASSERT_NULL(th->eq->eq->sup);
    TEST_ASSERT_EQUAL_CHAR('r', th->eq->eq->label);
    TEST_ASSERT(th->eq->eq->value == VALFIN);

    deleteTH(&th);
    TEST_ASSERT_NULL(th);
}

/* Black box test, results on valgrind */
void test_tiny(void)
{
    TrieHybride *th = newTH();
    int p = 1;
    th = ajoutTH(th, "hello", p++);
    th = ajoutTH(th, "help", p++);
    th = ajoutTH(th, "helt", p++);
    th = ajoutTH(th, "helm", p++);
    th = supprTH(th, "help");
    deleteTH(&th);
}

/* Black box test, results on valgrind */
void test_base(void)
{
    char *const basic_example_priv = malloc((strlen(basic_example) + 1) * sizeof(*basic_example_priv));
    if (!basic_example_priv)
        TEST_FAIL_MESSAGE("Erreur, malloc");
    strcpy(basic_example_priv, basic_example);

    TrieHybride *th = newTH();
    char *curr = strtok(basic_example_priv, " ");
    while (curr)
    {
        th = ajoutTH(th, curr, VALFIN);
        curr = strtok(NULL, " ");
    }

    deleteTH(&th);
    free(basic_example_priv);
}

/* Black box test, results on valgrind */
void test_base_suppr(void)
{
    char *const basic_example_priv = malloc((strlen(basic_example) + 1) * sizeof(*basic_example_priv));
    if (!basic_example_priv)
        TEST_FAIL_MESSAGE("Erreur, malloc");
    strcpy(basic_example_priv, basic_example);

    TrieHybride *th = newTH();
    char *curr = strtok(basic_example_priv, " ");
    while (curr)
    {
        th = ajoutTH(th, curr, VALFIN);
        curr = strtok(NULL, " ");
    }

    strcpy(basic_example_priv, basic_example);
    curr = strtok(basic_example_priv, " ");
    while (curr)
    {
        th = supprTH(th, curr);
        curr = strtok(NULL, " ");
    }

    deleteTH(&th);
    free(basic_example_priv);
}

/* Black box test, results on valgrind */
void test_shakespeare(void)
{
    const char *const dir_path = "test/Shakespeare/";
    const size_t dir_size = strlen(dir_path);

    DIR *dir;
    if ((dir = opendir(dir_path)) == NULL)
        TEST_FAIL_MESSAGE("Erreur, opendir");

    TrieHybride *th = newTH();

    struct dirent *dirent;
    errno = 0;
    char path_buf[PATH_MAX];
    while ((dirent = readdir(dir)) != NULL)
    {
        if (strstr(dirent->d_name, ".txt") == NULL)
            continue;

        snprintf(path_buf, PATH_MAX, "%.*s%s", (int)dir_size, dir_path, dirent->d_name);

        struct stat stat_buf;
        if (stat(path_buf, &stat_buf) == -1)
            TEST_FAIL_MESSAGE("Erreur, stat");

        int fd;
        if ((fd = open(path_buf, O_RDONLY)) == -1)
            TEST_FAIL_MESSAGE("Erreur, open");

        char *buf = malloc((stat_buf.st_size + 1) * sizeof(*buf));
        buf[stat_buf.st_size] = '\0';
        if (buf == NULL)
            TEST_FAIL_MESSAGE("Erreur, malloc");

        if (read(fd, buf, stat_buf.st_size) < stat_buf.st_size)
            TEST_FAIL_MESSAGE("Erreur, read");

        char *curr = strtok(buf, "\n");
        while (curr)
        {
            th = ajoutTH(th, curr, VALFIN);
            curr = strtok(NULL, "\n");
        }

        if (close(fd) == -1)
            TEST_FAIL_MESSAGE("Erreur, close");
        free(buf);
    }
    if (errno)
        TEST_FAIL_MESSAGE("Erreur, readdir");

    if (closedir(dir) == -1)
        TEST_FAIL_MESSAGE("Erreur, closedir");

    deleteTH(&th);
}

/* Black box test, results on valgrind */
void test_shakespeare_suppr(void)
{
    const char *const dir_path = "test/Shakespeare/";
    const size_t dir_size = strlen(dir_path);

    DIR *dir;
    if ((dir = opendir(dir_path)) == NULL)
        TEST_FAIL_MESSAGE("Erreur, opendir");

    TrieHybride *th = newTH();

    struct dirent *dirent;
    errno = 0;
    char path_buf[PATH_MAX];
    while ((dirent = readdir(dir)) != NULL)
    {
        if (strstr(dirent->d_name, ".txt") == NULL)
            continue;

        snprintf(path_buf, PATH_MAX, "%.*s%s", (int)dir_size, dir_path, dirent->d_name);

        struct stat stat_buf;
        if (stat(path_buf, &stat_buf) == -1)
            TEST_FAIL_MESSAGE("Erreur, stat");

        int fd;
        if ((fd = open(path_buf, O_RDONLY)) == -1)
            TEST_FAIL_MESSAGE("Erreur, open");

        char *buf = malloc((stat_buf.st_size + 1) * sizeof(*buf));
        buf[stat_buf.st_size] = '\0';
        if (buf == NULL)
            TEST_FAIL_MESSAGE("Erreur, malloc");

        if (read(fd, buf, stat_buf.st_size) < stat_buf.st_size)
            TEST_FAIL_MESSAGE("Erreur, read");

        char *curr = strtok(buf, "\n");
        while (curr)
        {
            th = ajoutTH(th, curr, VALFIN);
            curr = strtok(NULL, "\n");
        }

        if (close(fd) == -1)
            TEST_FAIL_MESSAGE("Erreur, close");
        free(buf);
    }
    if (errno)
        TEST_FAIL_MESSAGE("Erreur, readdir");

    rewinddir(dir);
    while ((dirent = readdir(dir)) != NULL)
    {
        if (strstr(dirent->d_name, ".txt") == NULL)
            continue;

        snprintf(path_buf, PATH_MAX, "%s%s", dir_path, dirent->d_name);

        struct stat stat_buf;
        if (stat(path_buf, &stat_buf) == -1)
            TEST_FAIL_MESSAGE("Erreur, stat");

        int fd;
        if ((fd = open(path_buf, O_RDONLY)) == -1)
            TEST_FAIL_MESSAGE("Erreur, open");

        char *buf = malloc((stat_buf.st_size + 1) * sizeof(*buf));
        buf[stat_buf.st_size] = '\0';
        if (buf == NULL)
            TEST_FAIL_MESSAGE("Erreur, malloc");

        if (read(fd, buf, stat_buf.st_size) < stat_buf.st_size)
            TEST_FAIL_MESSAGE("Erreur, read");

        char *curr = strtok(buf, "\n");
        while (curr)
        {
            th = supprTH(th, curr);
            curr = strtok(NULL, "\n");
        }

        if (close(fd) == -1)
            TEST_FAIL_MESSAGE("Erreur, close");
        free(buf);
    }
    if (errno)
        TEST_FAIL_MESSAGE("Erreur, readdir");

    if (closedir(dir) == -1)
        TEST_FAIL_MESSAGE("Erreur, closedir");

    TEST_ASSERT_NULL(th);
    deleteTH(&th);
}

/* Black box test, results on valgrind */
void test_huge(void)
{
    const char *const path = "test/words.txt";
    TrieHybride *th = newTH();

    int fd;
    if ((fd = open(path, O_RDONLY)) == -1)
        TEST_FAIL_MESSAGE("Erreur, open");

    struct stat stat_buf;
    if (fstat(fd, &stat_buf) == -1)
        TEST_FAIL_MESSAGE("Erreur, stat");

    char *buf = malloc((stat_buf.st_size + 1) * sizeof(*buf));
    buf[stat_buf.st_size] = '\0';
    if (buf == NULL)
        TEST_FAIL_MESSAGE("Erreur, malloc");

    if (read(fd, buf, stat_buf.st_size) < stat_buf.st_size)
        TEST_FAIL_MESSAGE("Erreur, read");

    char *curr = strtok(buf, "\n");
    while (curr)
    {
        th = ajoutTH(th, curr, VALFIN);
        curr = strtok(NULL, "\n");
    }

    if (close(fd) == -1)
        TEST_FAIL_MESSAGE("Erreur, close");
    free(buf);
    deleteTH(&th);
}

/* Black box test, results on valgrind */
void test_huge_suppr(void)
{
    const char *const path = "test/words.txt";
    TrieHybride *th = newTH();

    int fd;
    if ((fd = open(path, O_RDONLY)) == -1)
        TEST_FAIL_MESSAGE("Erreur, open");

    struct stat stat_buf;
    if (fstat(fd, &stat_buf) == -1)
        TEST_FAIL_MESSAGE("Erreur, stat");

    char *buf = malloc((stat_buf.st_size + 1) * sizeof(*buf));
    buf[stat_buf.st_size] = '\0';
    if (buf == NULL)
        TEST_FAIL_MESSAGE("Erreur, malloc");

    if (read(fd, buf, stat_buf.st_size) < stat_buf.st_size)
        TEST_FAIL_MESSAGE("Erreur, read");

    char *curr = strtok(buf, "\n");
    while (curr)
    {
        th = ajoutTH(th, curr, VALFIN);
        curr = strtok(NULL, "\n");
    }

    if (lseek(fd, 0, SEEK_SET) == -1)
        TEST_FAIL_MESSAGE("Erreur, lseek");

    if (read(fd, buf, stat_buf.st_size) < stat_buf.st_size)
        TEST_FAIL_MESSAGE("Erreur, read");

    curr = strtok(buf, "\n");
    while (curr)
    {
        th = supprTH(th, curr);
        curr = strtok(NULL, "\n");
    }

    if (close(fd) == -1)
        TEST_FAIL_MESSAGE("Erreur, close");
    free(buf);
    TEST_ASSERT_NULL(th);
    deleteTH(&th);
}

/* Black box test
 *
 * case #1:
 * search key in an empty trie
 */
void test_f_rechercheTH_1(void)
{
    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);
    TEST_ASSERT_FALSE(rechercheTH(th, "LeBron"));
    deleteTH(&th);
}

/* Black box test
 *
 * case #2:
 * search only key in a trie
 */
void test_f_rechercheTH_2(void)
{
    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);
    th = ajoutTH(th, "LeBron", VALFIN);
    TEST_ASSERT_NOT_NULL(th);
    TEST_ASSERT_TRUE(rechercheTH(th, "LeBron"));
    deleteTH(&th);
}

/* Black box test
 *
 * case #3:
 * search one of the two keys that share a prefix
 */
void test_f_rechercheTH_3(void)
{
    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);
    th = ajoutTH(th, "help", VALFIN);
    TEST_ASSERT_NOT_NULL(th);
    th = ajoutTH(th, "helm", VALFIN);
    TEST_ASSERT_NOT_NULL(th);
    TEST_ASSERT_TRUE(rechercheTH(th, "helm"));
    deleteTH(&th);
}

/* Black box test
 *
 * case #4:
 * search one of the two keys that share a prefix
 */
void test_f_rechercheTH_4(void)
{
    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);
    th = ajoutTH(th, "help", VALFIN);
    TEST_ASSERT_NOT_NULL(th);
    th = ajoutTH(th, "helm", VALFIN);
    TEST_ASSERT_NOT_NULL(th);
    TEST_ASSERT_TRUE(rechercheTH(th, "help"));
    deleteTH(&th);
}

/* Black box test
 *
 * case #5:
 * search the whole works of Shakespeare
 */
void test_f_rechercheTH_shakes(void)
{
    const char *const dir_path = "test/Shakespeare/";
    const size_t dir_size = strlen(dir_path);

    DIR *dir;
    if ((dir = opendir(dir_path)) == NULL)
        TEST_FAIL_MESSAGE("Erreur, opendir");

    TrieHybride *th = newTH();

    struct dirent *dirent;
    errno = 0;
    char path_buf[PATH_MAX];
    while ((dirent = readdir(dir)) != NULL)
    {
        if (strstr(dirent->d_name, ".txt") == NULL)
            continue;

        snprintf(path_buf, PATH_MAX, "%.*s%s", (int)dir_size, dir_path, dirent->d_name);

        struct stat stat_buf;
        if (stat(path_buf, &stat_buf) == -1)
            TEST_FAIL_MESSAGE("Erreur, stat");

        int fd;
        if ((fd = open(path_buf, O_RDONLY)) == -1)
            TEST_FAIL_MESSAGE("Erreur, open");

        char *buf = malloc((stat_buf.st_size + 1) * sizeof(*buf));
        buf[stat_buf.st_size] = '\0';
        if (buf == NULL)
            TEST_FAIL_MESSAGE("Erreur, malloc");

        if (read(fd, buf, stat_buf.st_size) < stat_buf.st_size)
            TEST_FAIL_MESSAGE("Erreur, read");

        char *curr = strtok(buf, "\n");
        while (curr)
        {
            th = ajoutTH(th, curr, VALFIN);
            curr = strtok(NULL, "\n");
        }

        if (close(fd) == -1)
            TEST_FAIL_MESSAGE("Erreur, close");
        free(buf);
    }
    if (errno)
        TEST_FAIL_MESSAGE("Erreur, readdir");

    rewinddir(dir);
    while ((dirent = readdir(dir)) != NULL)
    {
        if (strstr(dirent->d_name, ".txt") == NULL)
            continue;

        snprintf(path_buf, PATH_MAX, "%s%s", dir_path, dirent->d_name);

        struct stat stat_buf;
        if (stat(path_buf, &stat_buf) == -1)
            TEST_FAIL_MESSAGE("Erreur, stat");

        int fd;
        if ((fd = open(path_buf, O_RDONLY)) == -1)
            TEST_FAIL_MESSAGE("Erreur, open");

        char *buf = malloc((stat_buf.st_size + 1) * sizeof(*buf));
        buf[stat_buf.st_size] = '\0';
        if (buf == NULL)
            TEST_FAIL_MESSAGE("Erreur, malloc");

        if (read(fd, buf, stat_buf.st_size) < stat_buf.st_size)
            TEST_FAIL_MESSAGE("Erreur, read");

        char *curr = strtok(buf, "\n");
        while (curr)
        {
            TEST_ASSERT_TRUE(rechercheTH(th, curr));
            curr = strtok(NULL, "\n");
        }

        if (close(fd) == -1)
            TEST_FAIL_MESSAGE("Erreur, close");
        free(buf);
    }
    if (errno)
        TEST_FAIL_MESSAGE("Erreur, readdir");

    if (closedir(dir) == -1)
        TEST_FAIL_MESSAGE("Erreur, closedir");

    deleteTH(&th);
}

/* Black box test
 *
 * case #1:
 * count the keys of an empty trie
 */
void test_f_comptageMotsTH_1(void)
{
    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);
    TEST_ASSERT_EQUAL(0, comptageMotsTH(th));
    deleteTH(&th);
}

/* Black box test
 *
 * case #2:
 * count the keys of a complex trie
 */
void test_f_comptageMotsTH_2(void)
{
    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);
    th = ajoutTH(th, "hello", VALFIN);
    th = ajoutTH(th, "help", VALFIN);
    th = ajoutTH(th, "helt", VALFIN);
    th = ajoutTH(th, "helm", VALFIN);
    th = ajoutTH(th, "zoomin", VALFIN);
    TEST_ASSERT_EQUAL(5, comptageMotsTH(th));
    deleteTH(&th);
}

/* Black box test
 *
 * case #3:
 * count the unique words in the whole works of Shakespeare
 */
void test_f_comptageMotsTH_shakes(void)
{
    const char *const dir_path = "test/Shakespeare/";
    const size_t dir_size = strlen(dir_path);

    DIR *dir;
    if ((dir = opendir(dir_path)) == NULL)
        TEST_FAIL_MESSAGE("Erreur, opendir");

    TrieHybride *th = newTH();

    struct dirent *dirent;
    errno = 0;
    char path_buf[PATH_MAX];
    while ((dirent = readdir(dir)) != NULL)
    {
        if (strstr(dirent->d_name, ".txt") == NULL)
            continue;

        snprintf(path_buf, PATH_MAX, "%.*s%s", (int)dir_size, dir_path, dirent->d_name);

        struct stat stat_buf;
        if (stat(path_buf, &stat_buf) == -1)
            TEST_FAIL_MESSAGE("Erreur, stat");

        int fd;
        if ((fd = open(path_buf, O_RDONLY)) == -1)
            TEST_FAIL_MESSAGE("Erreur, open");

        char *buf = malloc((stat_buf.st_size + 1) * sizeof(*buf));
        buf[stat_buf.st_size] = '\0';
        if (buf == NULL)
            TEST_FAIL_MESSAGE("Erreur, malloc");

        if (read(fd, buf, stat_buf.st_size) < stat_buf.st_size)
            TEST_FAIL_MESSAGE("Erreur, read");

        char *curr = strtok(buf, "\n");
        while (curr)
        {
            th = ajoutTH(th, curr, VALFIN);
            curr = strtok(NULL, "\n");
        }

        if (close(fd) == -1)
            TEST_FAIL_MESSAGE("Erreur, close");
        free(buf);
    }
    if (errno)
        TEST_FAIL_MESSAGE("Erreur, readdir");

    // cat test/Shakespeare/*.txt | sort | uniq | wc -w
    TEST_ASSERT_EQUAL(23086, comptageMotsTH(th));

    if (closedir(dir) == -1)
        TEST_FAIL_MESSAGE("Erreur, closedir");

    deleteTH(&th);
}

/* Black box test
 *
 * case #1:
 * calculate height of an empty tree
 */
void test_f_hauteurTH_1(void)
{
    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);
    TEST_ASSERT_EQUAL(-1, hauteurTH(th));
    deleteTH(&th);
}

/* Black box test
 *
 * case #2:
 * calculate height of a tree containing a single key
 */
void test_f_hauteurTH_2(void)
{
    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);
    th = ajoutTH(th, "hello", VALFIN);
    TEST_ASSERT_NOT_NULL(th);
    TEST_ASSERT_EQUAL(4, hauteurTH(th));
    deleteTH(&th);
}

/* Black box test
 *
 * case #3:
 * calculate height of a complex tree containing multiple keys
 */
void test_f_hauteurTH_3(void)
{
    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);
    th = ajoutTH(th, "hello", VALFIN);
    th = ajoutTH(th, "help", VALFIN);
    th = ajoutTH(th, "helm", VALFIN);
    th = ajoutTH(th, "ada", VALFIN);
    TEST_ASSERT_NOT_NULL(th);
    TEST_ASSERT_EQUAL(5, hauteurTH(th));
    deleteTH(&th);
}

/* Black box test
 *
 * case #1:
 * alphabetically list the keys of an emtpy trie
 */
void test_f_listeMotsTH_1(void)
{
    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);
    char **res = listeMotsTH(th);
    TEST_ASSERT_NULL(res);
    deleteListeMotsTH(res);
    deleteTH(&th);
}

/* Black box test
 *
 * case #2:
 * alphabetically list the keys of a complex trie
 */
void test_f_listeMotsTH_2(void)
{
    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);

    th = ajoutTH(th, "hello", VALFIN);
    th = ajoutTH(th, "help", VALFIN);
    th = ajoutTH(th, "helm", VALFIN);
    th = ajoutTH(th, "ada", VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    char **res = listeMotsTH(th);
    TEST_ASSERT_NOT_NULL(res);
    TEST_ASSERT_NOT_NULL(res[0]);
    TEST_ASSERT_NOT_NULL(res[1]);
    TEST_ASSERT_NOT_NULL(res[2]);
    TEST_ASSERT_NOT_NULL(res[3]);
    TEST_ASSERT_NULL(res[4]);

    TEST_ASSERT_EQUAL_STRING("ada", res[0]);
    TEST_ASSERT_EQUAL_STRING("hello", res[1]);
    TEST_ASSERT_EQUAL_STRING("helm", res[2]);
    TEST_ASSERT_EQUAL_STRING("help", res[3]);

    deleteListeMotsTH(res);
    deleteTH(&th);
}

/* Black box test
 *
 * case #3:
 * alphabetically list the keys of a complex trie
 */
void test_f_listeMotsTH_3(void)
{
    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);

    th = ajoutTH(th, "car", VALFIN);
    th = ajoutTH(th, "cat", VALFIN);
    th = ajoutTH(th, "cart", VALFIN);
    th = ajoutTH(th, "dog", VALFIN);
    th = ajoutTH(th, "bat", VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    char **res = listeMotsTH(th);
    TEST_ASSERT_NOT_NULL(res);
    TEST_ASSERT_NOT_NULL(res[0]);
    TEST_ASSERT_NOT_NULL(res[1]);
    TEST_ASSERT_NOT_NULL(res[2]);
    TEST_ASSERT_NOT_NULL(res[3]);
    TEST_ASSERT_NOT_NULL(res[4]);
    TEST_ASSERT_NULL(res[5]);

    TEST_ASSERT_EQUAL_STRING("bat", res[0]);
    TEST_ASSERT_EQUAL_STRING("car", res[1]);
    TEST_ASSERT_EQUAL_STRING("cart", res[2]);
    TEST_ASSERT_EQUAL_STRING("cat", res[3]);
    TEST_ASSERT_EQUAL_STRING("dog", res[4]);

    deleteListeMotsTH(res);
    deleteTH(&th);
}

/* Black box test
 *
 * case #1:
 * count null pointers on an empty trie
 */
void test_f_comptageNilTH_1(void)
{
    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);

    TEST_ASSERT_EQUAL(1, comptageNilTH(th));

    deleteTH(&th);
}

/* Black box test
 *
 * case #2:
 * count null pointers on a complex trie
 */
void test_f_comptageNilTH_2(void)
{
    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);

    th = ajoutTH(th, "hello", VALFIN);
    th = ajoutTH(th, "help", VALFIN);
    th = ajoutTH(th, "helm", VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    TEST_ASSERT_EQUAL(15, comptageNilTH(th));

    deleteTH(&th);
}

/* Black box test
 *
 * case #1:
 * calculate the average depth of an empty trie
 */
void test_f_profondeurMoyenneEntTH_1(void)
{
    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);

    TEST_ASSERT_EQUAL(-1, profondeurMoyenneEntTH(th));

    deleteTH(&th);
}

/* Black box test
 *
 * case #2:
 * calculate the average depth of a complex trie
 */
void test_f_profondeurMoyenneEntTH_2(void)
{
    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);

    th = ajoutTH(th, "hello", VALFIN);
    th = ajoutTH(th, "help", VALFIN);
    th = ajoutTH(th, "helm", VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    TEST_ASSERT_EQUAL(4, profondeurMoyenneEntTH(th));

    deleteTH(&th);
}

/* Black box test
 *
 * case #1:
 * calculate the average depth of an empty trie
 */
void test_f_profondeurMoyenneTH_1(void)
{
    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);

    TEST_ASSERT(profondeurMoyenneTH(th) < 0);

    deleteTH(&th);
}

/* Black box test
 *
 * case #2:
 * calculate the average depth of a complex trie
 */
void test_f_profondeurMoyenneTH_2(void)
{
    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);

    th = ajoutTH(th, "hello", VALFIN);
    th = ajoutTH(th, "help", VALFIN);
    th = ajoutTH(th, "helm", VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    TEST_ASSERT_EQUAL(4.5, profondeurMoyenneTH(th));

    deleteTH(&th);
}

/* Black box test
 *
 * case #1:
 * count how many words' prefix is a given word in an empty trie
 */
void test_f_prefixeTH_1(void)
{
    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);

    TEST_ASSERT_EQUAL(0, prefixeTH(th, "LeBron"));

    deleteTH(&th);
}

/* Black box test
 *
 * case #2:
 * count how many words' prefix is a given word in a complex trie,
 * the given word is not in the trie,
 * the given word is completely unrelated to any words in the trie
 */
void test_f_prefixeTH_2(void)
{
    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);

    th = ajoutTH(th, "hello", VALFIN);
    th = ajoutTH(th, "help", VALFIN);
    th = ajoutTH(th, "helm", VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    TEST_ASSERT_EQUAL(0, prefixeTH(th, "pro"));

    deleteTH(&th);
}

/* Black box test
 *
 * case #3:
 * count how many words' prefix is a given word in a complex trie,
 * the given word is not in the trie,
 * the given word is partially related to the first word in the trie,
 * the given word is shorter than the first word in the trie
 */
void test_f_prefixeTH_3(void)
{
    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);

    th = ajoutTH(th, "hello", VALFIN);
    th = ajoutTH(th, "help", VALFIN);
    th = ajoutTH(th, "helm", VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    TEST_ASSERT_EQUAL(0, prefixeTH(th, "her"));

    deleteTH(&th);
}

/* Black box test
 *
 * case #4:
 * count how many words' prefix is a given word in a complex trie,
 * the given word is not in the trie,
 * the given word is partially related to the first word in the trie,
 * the given word is longer than the first word in the trie
 */
void test_f_prefixeTH_4(void)
{
    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);

    th = ajoutTH(th, "hello", VALFIN);
    th = ajoutTH(th, "help", VALFIN);
    th = ajoutTH(th, "helm", VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    TEST_ASSERT_EQUAL(0, prefixeTH(th, "helloh"));

    deleteTH(&th);
}

/* Black box test
 *
 * case #5:
 * count how many words' prefix is a given word in a complex trie,
 * the given word is not in the trie,
 * the given word is partially related to the first word in the trie,
 * the given word is the same length as the first word in the trie
 */
void test_f_prefixeTH_5(void)
{
    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);

    th = ajoutTH(th, "hello", VALFIN);
    th = ajoutTH(th, "help", VALFIN);
    th = ajoutTH(th, "helm", VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    TEST_ASSERT_EQUAL(0, prefixeTH(th, "hella"));

    deleteTH(&th);
}

/* Black box test
 *
 * case #6:
 * count how many words' prefix is a given word in a complex trie,
 * the given word is in the trie
 */
void test_f_prefixeTH_6(void)
{
    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);

    th = ajoutTH(th, "hello", VALFIN);
    th = ajoutTH(th, "help", VALFIN);
    th = ajoutTH(th, "helm", VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    TEST_ASSERT_EQUAL(1, prefixeTH(th, "hello"));

    deleteTH(&th);
}

/* Black box test
 *
 * case #7:
 * count how many words' prefix is a given word in a complex trie,
 * the given word is in the trie
 */
void test_f_prefixeTH_7(void)
{
    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);

    th = ajoutTH(th, "hello", VALFIN);
    th = ajoutTH(th, "help", VALFIN);
    th = ajoutTH(th, "helm", VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    TEST_ASSERT_EQUAL(1, prefixeTH(th, "hell"));

    deleteTH(&th);
}

/* Black box test
 *
 * case #8:
 * count how many words' prefix is a given word in a complex trie,
 * the given word is in the trie
 */
void test_f_prefixeTH_8(void)
{
    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);

    th = ajoutTH(th, "hello", VALFIN);
    th = ajoutTH(th, "help", VALFIN);
    th = ajoutTH(th, "helm", VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    TEST_ASSERT_EQUAL(3, prefixeTH(th, "hel"));

    deleteTH(&th);
}

/* Black box test
 *
 * case #9:
 * count how many words' prefix is a given word in a complex trie,
 * the given word is in the trie
 */
void test_f_prefixeTH_9(void)
{
    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);

    th = ajoutTH(th, "hello", VALFIN);
    th = ajoutTH(th, "help", VALFIN);
    th = ajoutTH(th, "helm", VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    TEST_ASSERT_EQUAL(1, prefixeTH(th, "help"));

    deleteTH(&th);
}

/* Black box test
 *
 * case #10:
 * count how many words' prefix is a given word in a complex trie,
 * the given word is in the trie
 */
void test_f_prefixeTH_10(void)
{
    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);

    th = ajoutTH(th, "hello", VALFIN);
    th = ajoutTH(th, "help", VALFIN);
    th = ajoutTH(th, "helm", VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    TEST_ASSERT_EQUAL(1, prefixeTH(th, "helm"));

    deleteTH(&th);
}

/* Black box test
 *
 * case #11:
 * count how many words' prefix is a given word in a complex trie,
 * the given word is in the trie
 */
void test_f_prefixeTH_11(void)
{
    TrieHybride *th = newTH();
    TEST_ASSERT_NULL(th);

    th = ajoutTH(th, "hello", VALFIN);
    th = ajoutTH(th, "help", VALFIN);
    th = ajoutTH(th, "helm", VALFIN);
    th = ajoutTH(th, "hel", VALFIN);
    TEST_ASSERT_NOT_NULL(th);

    TEST_ASSERT_EQUAL(4, prefixeTH(th, "hel"));

    deleteTH(&th);
}

/* Black box test
 *
 * case #1:
 * merge two empty tries
 */
void test_f_fusionTH_1(void)
{
    TrieHybride *th1 = newTH();
    TEST_ASSERT_NULL(th1);
    TrieHybride *th2 = newTH();
    TEST_ASSERT_NULL(th2);

    TrieHybride *tf = fusionTH(&th1, th2);
    TEST_ASSERT_NULL(tf);
    TEST_ASSERT_NULL(th1);

    deleteTH(&tf);
    deleteTH(&th2);
}

/* Black box test
 *
 * case #2:
 * merge two tries where one is empty
 */
void test_f_fusionTH_2(void)
{
    TrieHybride *th1 = newTH();
    TEST_ASSERT_NULL(th1);
    TrieHybride *th2 = newTH();
    TEST_ASSERT_NULL(th2);

    th1 = ajoutTH(th1, "car", VALFIN);
    th1 = ajoutTH(th1, "cat", VALFIN);
    TEST_ASSERT_NOT_NULL(th1);

    TrieHybride *tf = fusionTH(&th1, th2);
    TEST_ASSERT_NOT_NULL(tf);
    TEST_ASSERT_NULL(th1);

    char **l = listeMotsTH(tf);
    TEST_ASSERT_NOT_NULL(l);
    TEST_ASSERT_NOT_NULL(l[0]);
    TEST_ASSERT_NOT_NULL(l[1]);
    TEST_ASSERT_NULL(l[2]);
    TEST_ASSERT_EQUAL_STRING("car", l[0]);
    TEST_ASSERT_EQUAL_STRING("cat", l[1]);

    deleteListeMotsTH(l);
    deleteTH(&tf);
    deleteTH(&th2);
}

/* Black box test
 *
 * case #3:
 * merge two tries where one is empty
 */
void test_f_fusionTH_3(void)
{
    TrieHybride *th1 = newTH();
    TEST_ASSERT_NULL(th1);
    TrieHybride *th2 = newTH();
    TEST_ASSERT_NULL(th2);

    th1 = ajoutTH(th1, "car", VALFIN);
    th1 = ajoutTH(th1, "cat", VALFIN);
    TEST_ASSERT_NOT_NULL(th1);

    TrieHybride *tf = fusionTH(&th2, th1);
    TEST_ASSERT_NOT_NULL(tf);
    TEST_ASSERT_NULL(th2);

    char **l = listeMotsTH(tf);
    TEST_ASSERT_NOT_NULL(l);
    TEST_ASSERT_NOT_NULL(l[0]);
    TEST_ASSERT_NOT_NULL(l[1]);
    TEST_ASSERT_NULL(l[2]);
    TEST_ASSERT_EQUAL_STRING("car", l[0]);
    TEST_ASSERT_EQUAL_STRING("cat", l[1]);

    deleteListeMotsTH(l);
    deleteTH(&tf);
    deleteTH(&th1);
}

/* Black box test
 *
 * case #4:
 * merge two tries where none is empty
 */
void test_f_fusionTH_4(void)
{
    TrieHybride *th1 = newTH();
    TEST_ASSERT_NULL(th1);
    TrieHybride *th2 = newTH();
    TEST_ASSERT_NULL(th2);

    th1 = ajoutTH(th1, "car", VALFIN);
    th1 = ajoutTH(th1, "cat", VALFIN);
    TEST_ASSERT_NOT_NULL(th1);

    th2 = ajoutTH(th2, "cat", VALFIN);
    th2 = ajoutTH(th2, "dog", VALFIN);
    TEST_ASSERT_NOT_NULL(th2);

    TrieHybride *tf = fusionTH(&th1, th2);
    TEST_ASSERT_NOT_NULL(tf);
    TEST_ASSERT_NULL(th1);

    char **l = listeMotsTH(tf);
    TEST_ASSERT_NOT_NULL(l);
    TEST_ASSERT_NOT_NULL(l[0]);
    TEST_ASSERT_NOT_NULL(l[1]);
    TEST_ASSERT_NOT_NULL(l[2]);
    TEST_ASSERT_NULL(l[3]);
    TEST_ASSERT_EQUAL_STRING("car", l[0]);
    TEST_ASSERT_EQUAL_STRING("cat", l[1]);
    TEST_ASSERT_EQUAL_STRING("dog", l[2]);

    deleteListeMotsTH(l);
    deleteTH(&tf);
    deleteTH(&th2);
}

/* Black box test
 *
 * case #5:
 * merge two tries where none is empty,
 * possibly duplicate producing scenario for certain algorithm implementations
 */
void test_f_fusionTH_5(void)
{
    TrieHybride *th1 = newTH();
    TEST_ASSERT_NULL(th1);
    TrieHybride *th2 = newTH();
    TEST_ASSERT_NULL(th2);

    th1 = ajoutTH(th1, "cat", VALFIN);
    th1 = ajoutTH(th1, "bat", VALFIN);
    TEST_ASSERT_NOT_NULL(th1);

    th2 = ajoutTH(th2, "art", VALFIN);
    th2 = ajoutTH(th2, "bat", VALFIN);
    TEST_ASSERT_NOT_NULL(th2);

    TrieHybride *tf = fusionTH(&th1, th2);
    TEST_ASSERT_NOT_NULL(tf);
    TEST_ASSERT_NULL(th1);

    char **l = listeMotsTH(tf);
    TEST_ASSERT_NOT_NULL(l);
    TEST_ASSERT_NOT_NULL(l[0]);
    TEST_ASSERT_NOT_NULL(l[1]);
    TEST_ASSERT_NOT_NULL(l[2]);
    TEST_ASSERT_NULL(l[3]);
    TEST_ASSERT_EQUAL_STRING("art", l[0]);
    TEST_ASSERT_EQUAL_STRING("bat", l[1]);
    TEST_ASSERT_EQUAL_STRING("cat", l[2]);

    deleteListeMotsTH(l);
    deleteTH(&tf);
    deleteTH(&th2);
}

/* Black box test
 *
 * case #1:
 * merge two tries where none is empty,
 * possibly duplicate producing scenario for certain algorithm implementations
 */
void test_f_fusionCopieTH_1(void)
{
    TrieHybride *th1 = newTH();
    TEST_ASSERT_NULL(th1);
    TrieHybride *th2 = newTH();
    TEST_ASSERT_NULL(th2);

    th1 = ajoutTH(th1, "cat", VALFIN);
    th1 = ajoutTH(th1, "bat", VALFIN);
    TEST_ASSERT_NOT_NULL(th1);

    th2 = ajoutTH(th2, "art", VALFIN);
    th2 = ajoutTH(th2, "bat", VALFIN);
    TEST_ASSERT_NOT_NULL(th2);

    TrieHybride *tf = fusionCopieTH(th1, th2);
    TEST_ASSERT_NOT_NULL(tf);
    TEST_ASSERT_NOT_NULL(th1);
    TEST_ASSERT_NOT_NULL(th2);

    char **l = listeMotsTH(tf);
    TEST_ASSERT_NOT_NULL(l);
    TEST_ASSERT_NOT_NULL(l[0]);
    TEST_ASSERT_NOT_NULL(l[1]);
    TEST_ASSERT_NOT_NULL(l[2]);
    TEST_ASSERT_NULL(l[3]);
    TEST_ASSERT_EQUAL_STRING("art", l[0]);
    TEST_ASSERT_EQUAL_STRING("bat", l[1]);
    TEST_ASSERT_EQUAL_STRING("cat", l[2]);

    deleteListeMotsTH(l);
    deleteTH(&tf);
    deleteTH(&th1);
    deleteTH(&th2);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_f_prem);
    RUN_TEST(test_f_reste);
    RUN_TEST(test_f_car);
    RUN_TEST(test_f_lgueur);
    RUN_TEST(test_f_ajoutTH_1);
    RUN_TEST(test_f_ajoutTH_2);
    RUN_TEST(test_f_ajoutTH_3);
    RUN_TEST(test_f_ajoutTH_4);
    RUN_TEST(test_f_ajoutTH_5);
    RUN_TEST(test_f_ajoutTH_6);
    RUN_TEST(test_f_ajoutTH_7);
    RUN_TEST(test_f_ajoutTH_8);
    RUN_TEST(test_f_ajoutTH_9);
    RUN_TEST(test_f_supprTH_1);
    RUN_TEST(test_f_supprTH_2);
    RUN_TEST(test_f_supprTH_3);
    RUN_TEST(test_f_supprTH_4);
    RUN_TEST(test_f_supprTH_5);
    RUN_TEST(test_f_supprTH_6);
    RUN_TEST(test_f_supprTH_7);
    RUN_TEST(test_f_supprTH_8);
    RUN_TEST(test_f_supprTH_9);
    RUN_TEST(test_f_supprTH_10);
    RUN_TEST(test_f_supprTH_11);
    RUN_TEST(test_f_supprTH_12);
    RUN_TEST(test_f_supprTH_13);
    RUN_TEST(test_f_supprTH_14);
    RUN_TEST(test_f_supprTH_15);
    RUN_TEST(test_f_supprTH_16);
    RUN_TEST(test_f_supprTH_17);
    RUN_TEST(test_f_supprTH_18);
    RUN_TEST(test_f_supprTH_19);
    RUN_TEST(test_f_supprTH_20);
    RUN_TEST(test_f_supprTH_21);
    RUN_TEST(test_f_supprTH_22);
    RUN_TEST(test_f_supprTH_23);
    RUN_TEST(test_tiny);
    RUN_TEST(test_base);
    RUN_TEST(test_base_suppr);
    RUN_TEST(test_shakespeare);
    RUN_TEST(test_shakespeare_suppr);
    RUN_TEST(test_huge);
    RUN_TEST(test_huge_suppr);
    RUN_TEST(test_f_rechercheTH_1);
    RUN_TEST(test_f_rechercheTH_2);
    RUN_TEST(test_f_rechercheTH_3);
    RUN_TEST(test_f_rechercheTH_4);
    RUN_TEST(test_f_rechercheTH_shakes);
    RUN_TEST(test_f_comptageMotsTH_1);
    RUN_TEST(test_f_comptageMotsTH_2);
    RUN_TEST(test_f_comptageMotsTH_shakes);
    RUN_TEST(test_f_hauteurTH_1);
    RUN_TEST(test_f_hauteurTH_2);
    RUN_TEST(test_f_hauteurTH_3);
    RUN_TEST(test_f_listeMotsTH_1);
    RUN_TEST(test_f_listeMotsTH_2);
    RUN_TEST(test_f_listeMotsTH_3);
    RUN_TEST(test_f_comptageNilTH_1);
    RUN_TEST(test_f_comptageNilTH_2);
    RUN_TEST(test_f_profondeurMoyenneEntTH_1);
    RUN_TEST(test_f_profondeurMoyenneEntTH_2);
    RUN_TEST(test_f_profondeurMoyenneTH_1);
    RUN_TEST(test_f_profondeurMoyenneTH_2);
    RUN_TEST(test_f_prefixeTH_1);
    RUN_TEST(test_f_prefixeTH_2);
    RUN_TEST(test_f_prefixeTH_3);
    RUN_TEST(test_f_prefixeTH_4);
    RUN_TEST(test_f_prefixeTH_5);
    RUN_TEST(test_f_prefixeTH_6);
    RUN_TEST(test_f_prefixeTH_7);
    RUN_TEST(test_f_prefixeTH_8);
    RUN_TEST(test_f_prefixeTH_9);
    RUN_TEST(test_f_prefixeTH_10);
    RUN_TEST(test_f_prefixeTH_11);
    RUN_TEST(test_f_fusionTH_1);
    RUN_TEST(test_f_fusionTH_2);
    RUN_TEST(test_f_fusionTH_3);
    RUN_TEST(test_f_fusionTH_4);
    RUN_TEST(test_f_fusionTH_5);
    RUN_TEST(test_f_fusionCopieTH_1);
    return UNITY_END();
}
