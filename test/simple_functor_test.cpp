#include <simple_functor.h>

#include "gtest/gtest.h"

int free_func()
{
    return 42;
}

TEST(SimpleFunctorTest, StoreLambda)
{
    auto lambda_func = [](int i) { return i * i; };
    simple_functor<int(int)> functor{lambda_func};
    EXPECT_EQ(functor(2), 4);
}

TEST(SimpleFunctorTest, StoreFreeFunction)
{
    simple_functor<int()> functor{&free_func};
    EXPECT_EQ(functor(), 42);
}

TEST(SimpleFunctorTest, StoreFunctionObject)
{
    struct Functor
    {
        int operator()() { return v; }
        int v{8};
    };
    simple_functor<int()> functor{Functor{}};
    EXPECT_EQ(functor(), 8);
}

TEST(SimpleFunctorTest, StoreMemberFunction)
{
    struct Object
    {
        int get() { return v; }
        int v{1};
    };
    Object obj{};
    simple_functor<int()> functor{std::bind(&Object::get, &obj)};
    EXPECT_EQ(functor(), 1);
}
