#include <gtest/gtest.h>

// Demonstrate some basic assertions.
TEST(HelloTest2, BasicAssertions)
{
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world2");
    // Expect equality.
    EXPECT_EQ(1 + 1, 2);
}