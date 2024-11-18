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

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_create);
    return UNITY_END();
}
