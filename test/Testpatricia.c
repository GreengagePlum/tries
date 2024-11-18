#include "patricia.h"
#include "unity.h"

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
    int patricia = 0;
    TEST_ASSERT_EQUAL(0, patricia);
}

void test_rand(void)
{
    TEST_ASSERT_EQUAL(42, patricia(42));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_create);
    RUN_TEST(test_rand);
    return UNITY_END();
}
