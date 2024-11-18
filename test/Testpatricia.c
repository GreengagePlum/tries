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

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_create);
    return UNITY_END();
}
