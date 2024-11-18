#include "hybrid.h"
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
    int hybrid = 0; // creation
    TEST_ASSERT_EQUAL(0, hybrid);
}

void test_rand(void)
{
    TEST_ASSERT_EQUAL(42, hybrid(42));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_create);
    RUN_TEST(test_rand);
    return UNITY_END();
}
