#include <gtest/gtest.h>

TEST(SimpleMathTest, Addition) {
    int result = 1 + 1;
    EXPECT_EQ(result, 2);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

