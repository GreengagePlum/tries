#include "patricia.h"
#include "unity.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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
    PatriciaNode *node = create_patricia_node();
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_TRUE(0 == comptage_mots_patricia(node));
    free(node);
}

void test_insert(void)
{
    PatriciaNode *node = create_patricia_node();
    insert_patricia(node, "test");
    insert_patricia(node, "toto");

    TEST_ASSERT_EQUAL_STRING("t", node->prefixes[(unsigned char)'t']);
    TEST_ASSERT_EQUAL_STRING("est", node->children[(unsigned char)'t']->prefixes[(unsigned char)'e']);
    TEST_ASSERT_EQUAL_STRING("oto", node->children[(unsigned char)'t']->prefixes[(unsigned char)'o']);
    TEST_ASSERT_NULL(node->children[(unsigned char)'t']->children[(unsigned char)'e']);


    char *json = printJSONPT(node);
    printf("%s\n", json);

    TEST_ASSERT_TRUE(1 == recherche_patricia(node, "test"));
    TEST_ASSERT_TRUE(1 == recherche_patricia(node, "toto"));
    TEST_ASSERT_TRUE(0 == recherche_patricia(node, "t"));
    TEST_ASSERT_TRUE(2 == hauteur_patricia(node));
    TEST_ASSERT_TRUE(2 == comptage_mots_patricia(node));

    PatriciaNode *node2 = create_patricia_node();
    insert_patricia(node2, "apple");
    insert_patricia(node2, "application");
    insert_patricia(node2, "appetizer");

    node = pat_fusion(node, node2);
    TEST_ASSERT_EQUAL_STRING("t", node->prefixes[(unsigned char)'t']);
    TEST_ASSERT_EQUAL_STRING("est", node->children[(unsigned char)'t']->prefixes[(unsigned char)'e']);
    TEST_ASSERT_TRUE(5 == comptage_mots_patricia(node));

    TEST_ASSERT_TRUE(5 == comptage_mots_patricia(node));
    char** liste = liste_mots_patricia(node);
    print_list_patricia(liste, 5);
    free_list_patricia(liste, 5);

    free_patricia_node(node);
    free(json);
}

void test_insert2(void)
{
    PatriciaNode *node = create_patricia_node();
    insert_patricia(node, "the");
    insert_patricia(node, "they");
    TEST_ASSERT_TRUE(2 == comptage_mots_patricia(node));
    TEST_ASSERT_EQUAL_STRING("the", node->prefixes[(unsigned char)'t']);
    TEST_ASSERT_EQUAL_STRING("y", node->children[(unsigned char)'t']->prefixes[(unsigned char)'y']);
    TEST_ASSERT_NULL(node->children[(unsigned char)'t']->children[(unsigned char)'y']);
    TEST_ASSERT_EQUAL_STRING(" ", node->children[(unsigned char)'t']->prefixes[EOE_INDEX]);
    free_patricia_node(node);
    node = create_patricia_node();
    insert_patricia(node, "they");
    insert_patricia(node, "the");
    TEST_ASSERT_EQUAL_STRING("the", node->prefixes[(unsigned char)'t']);
    TEST_ASSERT_EQUAL_STRING("y", node->children[(unsigned char)'t']->prefixes[(unsigned char)'y']);
    TEST_ASSERT_EQUAL_STRING(" ", node->children[(unsigned char)'t']->prefixes[EOE_INDEX]);
    TEST_ASSERT_TRUE(2 == comptage_mots_patricia(node));
    int i = delete_word(node, "the");
    TEST_ASSERT_NULL(node->children[(unsigned char)'t']);
    TEST_ASSERT_TRUE(1 == i);
    print_patricia(node, 0);

    TEST_ASSERT_TRUE(1 == comptage_mots_patricia(node));
    i = delete_word(node, "they");
    TEST_ASSERT_TRUE(1 == i);
    TEST_ASSERT_TRUE(0 == comptage_mots_patricia(node));
    TEST_ASSERT_NULL(node->prefixes[(unsigned char)'t']);
    free_patricia_node(node);
}

void test_insert3(void)
{
    PatriciaNode *node = create_patricia_node();
    insert_patricia(node, "they");
    insert_patricia(node, "the");
    insert_patricia(node, "them");
    insert_patricia(node, "toto");
    insert_patricia(node, "there");
    TEST_ASSERT_EQUAL_STRING("t", node->prefixes[(unsigned char)'t']);
    TEST_ASSERT_TRUE(5 == comptage_mots_patricia(node));
    insert_patricia(node, "apple");
    insert_patricia(node, "application");
    insert_patricia(node, "appetizer");
    TEST_ASSERT_EQUAL_STRING("app", node->prefixes[(unsigned char)'a']);
    TEST_ASSERT_EQUAL_STRING("l", node->children[(unsigned char)'a']->prefixes[(unsigned char)'l']);
    TEST_ASSERT_EQUAL_STRING(
        "ication", node->children[(unsigned char)'a']->children[(unsigned char)'l']->prefixes[(unsigned char)'i']);
    TEST_ASSERT_EQUAL_STRING("etizer", node->children[(unsigned char)'a']->prefixes[(unsigned char)'e']);
    TEST_ASSERT_EQUAL_STRING(
        "e", node->children[(unsigned char)'a']->children[(unsigned char)'l']->prefixes[(unsigned char)'e']);
    TEST_ASSERT_TRUE(8 == comptage_mots_patricia(node));
    insert_patricia(node, "app");
    TEST_ASSERT_TRUE(9 == comptage_mots_patricia(node));


    TEST_ASSERT_TRUE(4 == nb_prefixe_patricia(node, "th"));
    TEST_ASSERT_TRUE(4 == nb_prefixe_patricia(node, "the"));
    TEST_ASSERT_TRUE(1 == nb_prefixe_patricia(node, "them"));
    TEST_ASSERT_TRUE(2 == nb_prefixe_patricia(node, "appl"));

    char** liste = liste_mots_patricia(node);
    print_list_patricia(liste, 9);
    free_list_patricia(liste, 9);

    TEST_ASSERT_EQUAL_STRING(" ", node->children[(unsigned char)'a']->prefixes[EOE_INDEX]);
    int i = delete_word(node, "app");
    TEST_ASSERT_TRUE(1 == i);
    i = delete_word(node, "appetizer");
    TEST_ASSERT_TRUE(1 == i);
    TEST_ASSERT_TRUE(7 == comptage_mots_patricia(node));
    TEST_ASSERT_EQUAL_STRING("appl", node->prefixes[(unsigned char)'a']);

    free_patricia_node(node);
}

void test_suffixe(void)
{
    const char *s1 = "pref";
    const char *s2 = "prefix";
    TEST_ASSERT_EQUAL_STRING("ix", suffixe(s1, s2));
}

void test_fusion(void)
{
    PatriciaNode *node = create_patricia_node();
    insert_patricia(node, "the");
    insert_patricia(node, "they");
    PatriciaNode *node2 = create_patricia_node();
    insert_patricia(node2, "the");
    insert_patricia(node2, "them");
    node = pat_fusion(node, node2);
    TEST_ASSERT_TRUE(3 == comptage_mots_patricia(node));
    free_patricia_node(node);

    PatriciaNode *empty1 = create_patricia_node();
    PatriciaNode *empty2 = create_patricia_node();
    PatriciaNode *result = pat_fusion(empty1, empty2);
    TEST_ASSERT_TRUE(0 == comptage_mots_patricia(result));
    free_patricia_node(result);

    empty1 = create_patricia_node();
    node = create_patricia_node();
    insert_patricia(node, "the");
    node = pat_fusion(empty1, node);
    TEST_ASSERT_TRUE(1 == comptage_mots_patricia(node));
    free_patricia_node(node);

    PatriciaNode *stress1 = create_patricia_node();
    PatriciaNode *stress2 = create_patricia_node();
    for (int i = 0; i < 1000; i++)
    {
        char word[50];
        sprintf(word, "word%d", i);
        insert_patricia(stress1, word);
        sprintf(word, "mot%d", i);
        insert_patricia(stress2, word);
    }
    result = pat_fusion(stress1, stress2);
    TEST_ASSERT_TRUE(2000 == comptage_mots_patricia(result));
    // char** liste = liste_mots_patricia(node);
    // print_list_patricia(liste, 2000);
    // free_list_patricia(liste, 2000);
    for(int i = 0; i < 1000; i++){
        char word[50];
        sprintf(word, "word%d", i);
        delete_word(result, word);
    }
    free_patricia_node(result);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_create);
    RUN_TEST(test_insert);
    RUN_TEST(test_insert2);
    RUN_TEST(test_insert3);
    RUN_TEST(test_fusion);

    RUN_TEST(test_suffixe);

    return UNITY_END();
}
