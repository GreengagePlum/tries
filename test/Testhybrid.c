#include "hybrid.h"
#include "unity.h"
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

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_tiny);
    RUN_TEST(test_base);
    return UNITY_END();
}
