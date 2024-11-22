#include "hybrid.h"
#include "unity.h"

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

void test_create(void)
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

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_create);
    return UNITY_END();
}
