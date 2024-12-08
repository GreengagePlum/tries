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
    free_patricia(node);
    
    }

void test_insert(void)
{
    PatriciaNode *node = create_patricia_node();
    insert_patricia(node, "test");
    insert_patricia(node, "toto");
    TEST_ASSERT_EQUAL_STRING("t", node->children[(unsigned char)'t']->label);
    TEST_ASSERT_EQUAL_STRING("est\x01", node->children[(unsigned char)'t']->children[(unsigned char)'e']->label);
    TEST_ASSERT_EQUAL_STRING("oto\x01", node->children[(unsigned char)'t']->children[(unsigned char)'o']->label);
    TEST_ASSERT_EQUAL(2, comptage_mots_patricia(node));

    free_patricia(node);
}


void test_insert2(void){
    PatriciaNode *node = create_patricia_node();
    insert_patricia(node, "the");
    insert_patricia(node, "they");
    insert_patricia(node, "them");
    insert_patricia(node, "toto");
    insert_patricia(node, "cat");
    insert_patricia(node, "car");
    insert_patricia(node, "cart");
    insert_patricia(node, "carp");
    insert_patricia(node, "carpet");
    insert_patricia(node, "carrot");
    insert_patricia(node, "carriage");
    insert_patricia(node, "caravan");
    insert_patricia(node, "carve");
    insert_patricia(node, "carving");   

    TEST_ASSERT_EQUAL_STRING("t", node->children[(unsigned char)'t']->label);
    TEST_ASSERT_EQUAL_STRING("he", node->children[(unsigned char)'t']->children[(unsigned char)'h']->label);
    TEST_ASSERT_EQUAL_STRING("y\x01", node->children[(unsigned char)'t']->children[(unsigned char)'h']->children[(unsigned char)'y']->label);
    TEST_ASSERT_EQUAL_STRING("\x01", node->children[(unsigned char)'t']->children[(unsigned char) 'h']->children[EOE_INDEX]->label);
    TEST_ASSERT_EQUAL_STRING("oto\x01", node->children[(unsigned char)'t']->children[(unsigned char)'o']->label);

    TEST_ASSERT_EQUAL_STRING("ca", node->children[(unsigned char)'c']->label);
    TEST_ASSERT_EQUAL_STRING("t\x01", node->children[(unsigned char)'c']->children[(unsigned char)'t']->label);
    TEST_ASSERT_EQUAL_STRING("r", node->children[(unsigned char)'c']->children[(unsigned char)'r']->label);
    TEST_ASSERT_EQUAL_STRING("t\x01", node->children[(unsigned char)'c']->children[(unsigned char)'r']->children[(unsigned char)'t']->label);
    TEST_ASSERT_EQUAL_STRING("p", node->children[(unsigned char)'c']->children[(unsigned char)'r']->children[(unsigned char)'p']->label);
    TEST_ASSERT_EQUAL_STRING("\x01", node->children[(unsigned char)'c']->children[(unsigned char)'r']->children[EOE_INDEX]->label);
    TEST_ASSERT_EQUAL_STRING("et\x01", node->children[(unsigned char)'c']->children[(unsigned char)'r']->children[(unsigned char)'p']->children[(unsigned char)'e']->label);
    TEST_ASSERT_EQUAL_STRING("r", node->children[(unsigned char)'c']->children[(unsigned char)'r']->label);
    TEST_ASSERT_EQUAL_STRING("ot\x01", node->children[(unsigned char)'c']->children[(unsigned char)'r']->children[(unsigned char)'r']->children[(unsigned char)'o']->label);
    TEST_ASSERT_EQUAL(true, recherche_patricia(node, "the"));
    TEST_ASSERT_EQUAL(true, recherche_patricia(node, "they"));
    TEST_ASSERT_EQUAL(true, recherche_patricia(node, "them"));
    TEST_ASSERT_EQUAL(true, recherche_patricia(node, "toto"));
    TEST_ASSERT_EQUAL(true, recherche_patricia(node, "cat"));
    TEST_ASSERT_EQUAL(true, recherche_patricia(node, "car"));
    TEST_ASSERT_EQUAL(true, recherche_patricia(node, "cart"));
    TEST_ASSERT_EQUAL(true, recherche_patricia(node, "carp"));
    TEST_ASSERT_EQUAL(true, recherche_patricia(node, "carpet"));
    TEST_ASSERT_EQUAL(true, recherche_patricia(node, "carrot"));
    TEST_ASSERT_EQUAL(true, recherche_patricia(node, "carriage"));
    TEST_ASSERT_EQUAL(true, recherche_patricia(node, "caravan"));
    TEST_ASSERT_EQUAL(true, recherche_patricia(node, "carve"));
    TEST_ASSERT_EQUAL(true, recherche_patricia(node, "carving"));
    TEST_ASSERT_EQUAL(false, recherche_patricia(node, "carvings"));

    TEST_ASSERT_EQUAL(14, comptage_mots_patricia(node));
    free_patricia(node);
}


int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_create);
    RUN_TEST(test_insert);
    RUN_TEST(test_insert2);

    return UNITY_END();
}
