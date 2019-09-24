#include <gtest/gtest.h>
#include "factor.h"

TEST(test_factor,eq){
    EXPECT_EQ(factor(0),1);
    EXPECT_EQ(factor(1),1);
    EXPECT_EQ(factor(2),2);
    EXPECT_EQ(factor(3),6);
    EXPECT_EQ(factor(4),24);
    EXPECT_EQ(factor(5),120);
    EXPECT_EQ(factor(6),720);
    EXPECT_EQ(factor(7),5040);
    EXPECT_EQ(factor(8),40320);
    EXPECT_EQ(factor(9),362880);
}