#include "hybrid.h"
#include "unity.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

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

    TrieHybride th = newTH();
    th = ajoutTH(th, cle, VALFIN);

    TEST_ASSERT_NOT_NULL(th);
    TrieHybride tmp = th;
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

    TrieHybride th = newTH();
    th = ajoutTH(th, cle1, VALFIN);
    th = ajoutTH(th, cle2, VALFIN);

    TrieHybride tmp = th;
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

    TrieHybride th = newTH();
    th = ajoutTH(th, cle1, VALFIN);
    th = ajoutTH(th, cle2, VALFIN);

    TrieHybride tmp = th;
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

    TrieHybride th = newTH();
    th = ajoutTH(th, cle1, VALFIN);
    th = ajoutTH(th, cle2, VALFIN);

    TrieHybride tmp = th;
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

    TrieHybride th = newTH();
    th = ajoutTH(th, cle1, VALFIN);
    th = ajoutTH(th, cle2, VALFIN);

    TrieHybride tmp = th;
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

    TrieHybride th = newTH();
    th = ajoutTH(th, cle1, VALFIN);
    th = ajoutTH(th, cle2, VALFIN);

    TrieHybride tmp = th;
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

    TrieHybride th = newTH();
    th = ajoutTH(th, cle1, VALFIN);
    th = ajoutTH(th, cle2, VALFIN);

    TrieHybride tmp = th;
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

    TrieHybride th = newTH();
    th = ajoutTH(th, cle1, VALFIN);
    th = ajoutTH(th, cle2, VALFIN);

    TrieHybride tmp = th;
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

    TrieHybride th = newTH();
    th = ajoutTH(th, cle, 1);
    th = ajoutTH(th, cle, 2);

    TrieHybride tmp = th;
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
    TrieHybride th = newTH();
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

    TrieHybride th = newTH();
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

    TrieHybride th = newTH();
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
}

/* White box test
 *
 * case #4:
 * delete existing key that is a prefix of another existing key
 */
void test_f_supprTH_4(void)
{
    const char *cle;

    TrieHybride th = newTH();
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
}

/* White box test
 *
 * case #5:
 * delete existing key that is a prefix of another existing key
 */
void test_f_supprTH_5(void)
{
    const char *cle;

    TrieHybride th = newTH();
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
}

/* White box test
 *
 * case #6:
 * delete inexistant key with no relation to any present keys of a populated trie
 */
void test_f_supprTH_6(void)
{
    const char *cle;

    TrieHybride th = newTH();
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
}

/* White box test
 *
 * case #7:
 * delete inexistant key that shares a prefix with a key of a populated trie
 */
void test_f_supprTH_7(void)
{
    const char *cle;

    TrieHybride th = newTH();
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
}

/* White box test
 *
 * case #8:
 * reparenting variations...
 */
void test_f_supprTH_8(void)
{
}

/* Black box test, results on valgrind */
void test_tiny(void)
{
    TrieHybride th = newTH();
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

    TrieHybride th = newTH();
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

    TrieHybride th = newTH();
    char *curr = strtok(basic_example_priv, " ");
    while (curr)
    {
        th = ajoutTH(th, curr, VALFIN);
        curr = strtok(NULL, " ");
    }

    curr = strtok(basic_example_priv, " ");
    while (curr)
    {
        th = supprTH(th, curr);
        curr = strtok(NULL, " ");
    }

    deleteTH(&th);
    free(basic_example_priv);
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
    RUN_TEST(test_tiny);
    RUN_TEST(test_base);
    RUN_TEST(test_base_suppr);
    return UNITY_END();
}
