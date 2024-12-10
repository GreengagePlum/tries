#include "patricia.h"
#include "unity.h"
#include <stdlib.h>
#include <string.h>
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
    free(node);
}

void test_insert(void)
{
    PatriciaNode *node = create_patricia_node();
    insert_patricia(node, "test");
    insert_patricia(node, "toto");
    TEST_ASSERT_EQUAL_STRING("t", node->prefixes[(unsigned char)'t']);
    TEST_ASSERT_EQUAL_STRING("est\x01", node->children[(unsigned char)'t']->prefixes[(unsigned char)'e']);
    TEST_ASSERT_EQUAL_STRING("oto\x01", node->children[(unsigned char)'t']->prefixes[(unsigned char)'o']);
    TEST_ASSERT_EQUAL(2, comptage_mots_patricia(node));
    char** liste = liste_mots_patricia(node);
    print_list_patricia(liste, 2);
    printf("Profondeur moyenne: %d\n", profondeur_moyenne_patricia_feuille(node));
    free(node);
}

void test_insert2(void){
    PatriciaNode *node = create_patricia_node();
    insert_patricia(node, "the");
    insert_patricia(node, "they");
    TEST_ASSERT_EQUAL_STRING("the", node->prefixes[(unsigned char)'t']);
    TEST_ASSERT_EQUAL_STRING("y\x01", node->children[(unsigned char)'t']->prefixes[(unsigned char)'y']);
    TEST_ASSERT_EQUAL(2, comptage_mots_patricia(node));
    char **liste = liste_mots_patricia(node);
    print_list_patricia(liste, 2);
    free(node);
}

void test_insert3(void){
    PatriciaNode *node = create_patricia_node();
    insert_patricia(node, "they");
    insert_patricia(node, "the");
    TEST_ASSERT_EQUAL_STRING("the", node->prefixes[(unsigned char)'t']);
    TEST_ASSERT_EQUAL_STRING("y\x01", node->children[(unsigned char)'t']->prefixes[(unsigned char)'y']);
    TEST_ASSERT_EQUAL_STRING("\x01", node->children[(unsigned char)'t']->prefixes[EOE_INDEX]);
    insert_patricia(node, "them");
    insert_patricia(node, "toto");
    insert_patricia(node, "there");
    TEST_ASSERT_EQUAL(5, comptage_mots_patricia(node));
    char **liste = liste_mots_patricia(node);
    print_list_patricia(liste, 5);
    /*
    TEST_ASSERT_EQUAL(1, delete_word(node, "the"));
    TEST_ASSERT_EQUAL_STRING("", node->children[(unsigned char)'t']->prefixes[EOE_INDEX]);
    TEST_ASSERT_EQUAL_STRING("they\x01", node->prefixes[(unsigned char)'t']);
    TEST_ASSERT_EQUAL(1, delete_word(node, "they"));
    TEST_ASSERT_NULL(node->prefixes[(unsigned char)'t']);
    */
    free(node);
}

void test_insert4(void){
    PatriciaNode *node = create_patricia_node();
    insert_patricia(node, "the");
    insert_patricia(node, "they");
    insert_patricia(node, "them");
    TEST_ASSERT_EQUAL_STRING("the", node->prefixes[(unsigned char)'t']);
    TEST_ASSERT_EQUAL_STRING("\x01", node->children[(unsigned char)'t']->prefixes[EOE_INDEX]);
    TEST_ASSERT_EQUAL_STRING("y\x01", node->children[(unsigned char)'t']->prefixes[(unsigned char)'y']);
    TEST_ASSERT_EQUAL_STRING("m\x01", node->children[(unsigned char)'t']->prefixes[(unsigned char)'m']);
    
    insert_patricia(node, "toto");
    TEST_ASSERT_EQUAL_STRING("t", node->prefixes[(unsigned char)'t']);
    TEST_ASSERT_EQUAL_STRING("oto\x01", node->children[(unsigned char)'t']->prefixes[(unsigned char)'o']);
    TEST_ASSERT_EQUAL_STRING("he", node->children[(unsigned char)'t']->prefixes[(unsigned char)'h']);
    TEST_ASSERT_EQUAL_STRING("m\x01", node->children[(unsigned char)'t']->children[(unsigned char)'h']->prefixes[(unsigned char)'m']);
    insert_patricia(node, "coffee");
    TEST_ASSERT_EQUAL_STRING("coffee\x01", node->prefixes[(unsigned char)'c']);
    TEST_ASSERT_EQUAL(5, comptage_mots_patricia(node));
    char **liste = liste_mots_patricia(node);
    print_list_patricia(liste, 5);
    free(node);}

void test_insert5(void){
    PatriciaNode *node = create_patricia_node();
    insert_patricia(node, "apple");
    insert_patricia(node, "application");
    insert_patricia(node, "appetizer");
    TEST_ASSERT_EQUAL_STRING("app", node->prefixes[(unsigned char)'a']);
    TEST_ASSERT_EQUAL_STRING("l", node->children[(unsigned char)'a']->prefixes[(unsigned char)'l']);
    TEST_ASSERT_EQUAL_STRING("ication\x01", node->children[(unsigned char)'a']->children[(unsigned char)'l']->prefixes[(unsigned char)'i']);
    TEST_ASSERT_EQUAL_STRING("etizer\x01", node->children[(unsigned char)'a']->prefixes[(unsigned char)'e']);
    TEST_ASSERT_EQUAL(3, comptage_mots_patricia(node));
    char **liste = liste_mots_patricia(node);   
    print_list_patricia(liste, 3);
}


void test_recherche(void) {
    PatriciaNode *node = create_patricia_node();
    insert_patricia(node, "the");
    insert_patricia(node, "they");
    insert_patricia(node, "them");
    insert_patricia(node, "toto");
    insert_patricia(node, "cat");
    insert_patricia(node, "car");
    insert_patricia(node, "cart");
    TEST_ASSERT_EQUAL(3, comptage_mots_patricia(node->children['t']->children['h']));
    TEST_ASSERT_EQUAL(7, comptage_mots_patricia(node));
    TEST_ASSERT_EQUAL_STRING("ca", node->prefixes[(unsigned char)'c']);
    TEST_ASSERT_EQUAL_STRING("r", node->children[(unsigned char)'c']->prefixes[(unsigned char)'r']);
    TEST_ASSERT_EQUAL_STRING("\x01", node->children[(unsigned char)'c']->children[(unsigned char)'r']->prefixes[EOE_INDEX]);
    TEST_ASSERT_EQUAL_STRING("t\x01", node->children[(unsigned char)'c']->children[(unsigned char)'r']->prefixes[(unsigned char)'t']);

    TEST_ASSERT_EQUAL(1, recherche_patricia(node, "cart"));
    TEST_ASSERT_EQUAL(1, recherche_patricia(node, "car"));
    TEST_ASSERT_EQUAL(1, recherche_patricia(node, "cat"));
    TEST_ASSERT_EQUAL(0, recherche_patricia(node,"vitamin"));
    TEST_ASSERT_EQUAL(1, recherche_patricia(node, "the")); 
    TEST_ASSERT_EQUAL(1, recherche_patricia(node, "they")); 
    TEST_ASSERT_EQUAL(1, recherche_patricia(node, "them")); 
    TEST_ASSERT_EQUAL(0, recherche_patricia(node, "there")); 
    TEST_ASSERT_EQUAL(0, recherche_patricia(node, "th")); 
    TEST_ASSERT_EQUAL(1, recherche_patricia(node, "toto"));

    TEST_ASSERT_EQUAL(4, nb_prefixe_patricia(node, "t"));
    TEST_ASSERT_EQUAL(3, nb_prefixe_patricia(node, "th"));
    TEST_ASSERT_EQUAL(3, nb_prefixe_patricia(node, "the"));
    TEST_ASSERT_EQUAL(1, nb_prefixe_patricia(node, "they"));
    TEST_ASSERT_EQUAL(3, nb_prefixe_patricia(node, "ca"));
    TEST_ASSERT_EQUAL(0, nb_prefixe_patricia(node, "carb"));

    char **liste = liste_mots_patricia(node);
    print_list_patricia(liste, 7);

    free(node);
}

void test_prefix(void){
    TEST_ASSERT_EQUAL(3, plus_long_pref("the", "the\x01"));
    TEST_ASSERT_EQUAL((int)strlen("the\x01"), plus_long_pref("the", "the\x01") + 1);
    TEST_ASSERT_EQUAL(1, plus_long_pref("y", "y\x01"));
    const char* s1 = "y";
    const char* prefix2 = "y\x01";
    int prefix_len = plus_long_pref(s1, prefix2);
    TEST_ASSERT_EQUAL('\x01' , prefix2[prefix_len]);
    TEST_ASSERT_EQUAL(1, prefix_len);
    TEST_ASSERT_EQUAL_STRING("\0", s1 += 1 );
    const char* s2 = "they";
    const char* prefix = "the";
    TEST_ASSERT_EQUAL(3, plus_long_pref(s2, prefix));
    TEST_ASSERT_EQUAL_STRING("y", s2 += 3);

    
}

void test_delete(void){
    PatriciaNode *node = create_patricia_node();
    insert_patricia(node, "test");
    insert_patricia(node, "testing");
    insert_patricia(node, "tested");
    int i = delete_word(node, "test");
    TEST_ASSERT_EQUAL(1, i);
    TEST_ASSERT_EQUAL(0, recherche_patricia(node, "test"));
    TEST_ASSERT_EQUAL_STRING("test", node->prefixes[(unsigned char)'t']);
    TEST_ASSERT_NULL(node->children[(unsigned char)'t']->prefixes[EOE_INDEX]);
    insert_patricia(node, "test");
    TEST_ASSERT_EQUAL(1, delete_word(node, "testing"));
    TEST_ASSERT_EQUAL_STRING("test", node->prefixes[(unsigned char)'t']);
    TEST_ASSERT_NOT_NULL(node->children[(unsigned char)'t']->prefixes[EOE_INDEX]);
    insert_patricia(node, "apple");
    insert_patricia(node, "application");
    insert_patricia(node, "appetizer");
    TEST_ASSERT_EQUAL(1, delete_word(node, "appetizer"));   
    TEST_ASSERT_EQUAL_STRING("appl", node->prefixes[(unsigned char)'a']);
    TEST_ASSERT_EQUAL_STRING("e\x01", node->children[(unsigned char)'a']->prefixes[(unsigned char)'e']);
    TEST_ASSERT_EQUAL_STRING("ication\x01", node->children[(unsigned char)'a']->children[(unsigned char)'e']);

}


int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_create);
    RUN_TEST(test_insert);
    RUN_TEST(test_insert2);
    RUN_TEST(test_insert3);
    RUN_TEST(test_insert4);
    RUN_TEST(test_insert5);
    RUN_TEST(test_recherche);
    RUN_TEST(test_prefix);
    RUN_TEST(test_delete);    
    return UNITY_END();
}
