#include "patricia.h"
#include "unity.h"
#include <stdlib.h>
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
    free(node);
}

void test_insert2(void){
    PatriciaNode *node = create_patricia_node();
    insert_patricia(node, "the");
    insert_patricia(node, "they");
    TEST_ASSERT_EQUAL_STRING("the", node->prefixes[(unsigned char)'t']);
    TEST_ASSERT_EQUAL_STRING("y\x01", node->children[(unsigned char)'t']->prefixes[(unsigned char)'y']);
    free(node);
}

void test_insert3(void){
    PatriciaNode *node = create_patricia_node();
    insert_patricia(node, "they");
    insert_patricia(node, "the");
    TEST_ASSERT_EQUAL_STRING("the", node->prefixes[(unsigned char)'t']);
    TEST_ASSERT_EQUAL_STRING("y\x01", node->children[(unsigned char)'t']->prefixes[(unsigned char)'y']);
    TEST_ASSERT_EQUAL_STRING("\x01", node->children[(unsigned char)'t']->prefixes[EOE_INDEX]);
    free(node);
}

void test_insert4(void){
    PatriciaNode *node = create_patricia_node();
    insert_patricia(node, "the");
    insert_patricia(node, "they");
    insert_patricia(node, "them");
    TEST_ASSERT_EQUAL_STRING("the", node->prefixes[(unsigned char)'t']);
    TEST_ASSERT_EQUAL_STRING("y\x01", node->children[(unsigned char)'t']->prefixes[(unsigned char)'y']);
    TEST_ASSERT_EQUAL_STRING("m\x01", node->children[(unsigned char)'t']->prefixes[(unsigned char)'m']);
    insert_patricia(node, "toto");
    TEST_ASSERT_EQUAL_STRING("t", node->prefixes[(unsigned char)'t']);
    TEST_ASSERT_EQUAL_STRING("oto\x01", node->children[(unsigned char)'t']->prefixes[(unsigned char)'o']);
    TEST_ASSERT_EQUAL_STRING("he", node->children[(unsigned char)'t']->prefixes[(unsigned char)'h']);
    TEST_ASSERT_EQUAL_STRING("m\x01", node->children[(unsigned char)'t']->children[(unsigned char)'h']->prefixes[(unsigned char)'m']);
    insert_patricia(node, "coffee");
    TEST_ASSERT_EQUAL_STRING("coffee\x01", node->prefixes[(unsigned char)'c']);
    free(node);}
int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_create);
    RUN_TEST(test_insert);
    RUN_TEST(test_insert2);
    RUN_TEST(test_insert3);
    RUN_TEST(test_insert4);
    return UNITY_END();
}
