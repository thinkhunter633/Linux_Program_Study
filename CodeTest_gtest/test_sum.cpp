#include <gtest/gtest.h>
extern "C" {
#include "sum.h"
}

TEST(SumTest, PositiveNumbers) {
    EXPECT_EQ(5, sum(2, 3));
    EXPECT_EQ(10, sum(5, 5));
    EXPECT_EQ(100, sum(50, 50));
}

TEST(multiplyTest, PositiveNumbers) {
    int iCurResult = 0;
    multiply(2, 3, &iCurResult);
    EXPECT_EQ(6, iCurResult);
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
