#include <simple_functor.h>

#include "gtest/gtest.h"

TEST(SimpleFunctorTest, StoreLambda)
{
    auto lambda_func = [](int i) { return i * i; };
    simple_functor<int(int)> functor{lambda_func};
    EXPECT_EQ(functor(2), 4);
}
