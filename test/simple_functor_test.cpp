#include <simple_functor.h>

#include "gtest/gtest.h"

int free_func()
{
    return 42;
}

struct Functor
{
    int operator()() { return v; }
    int v{8};
};

struct MoveOnlyFunctor
{
    MoveOnlyFunctor(const MoveOnlyFunctor &other) = delete;
    MoveOnlyFunctor(MoveOnlyFunctor &&other)      = default;
    MoveOnlyFunctor &operator=(MoveOnlyFunctor &&other) = default;
    MoveOnlyFunctor &operator=(const MoveOnlyFunctor &other) = delete;
    ~MoveOnlyFunctor()                                       = default;
    int operator()() { return v; }
    int v{8};
};

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

TEST(SimpleFunctorTest, CopySimpleFunctor)
{
    auto lambda_func = [](int i) { return i * i; };
    simple_functor<int(int)> functor{lambda_func};
    simple_functor<int(int)> functor_copy = functor;
    EXPECT_EQ(functor(2), 4);
    EXPECT_EQ(functor_copy(2), 4);
}

TEST(SimpleFunctorTest, MoveSimpleFunctor)
{
    auto lambda_func = [](int i) { return i * i; };
    simple_functor<int(int)> functor{lambda_func};
    simple_functor<int(int)> functor_moved{std::move(functor)};
    EXPECT_EQ(functor_moved(2), 4);
    EXPECT_DEATH(functor(2), ".*");
}

TEST(SimpleFunctorTest, StoreMoveOnlyFunction)
{
    auto move_only = MoveOnlyFunctor{};
    simple_functor<int()> functor{std::move(move_only)};
    EXPECT_EQ(functor(), 8);
}

TEST(SimpleFunctorTest, CopySimpleFunctorWithMoveOnlyFunctionInside)
{
    auto move_only = MoveOnlyFunctor{};
    simple_functor<int()> functor{std::move(move_only)};
    auto test_call = [&]() { simple_functor<int()> functor_copy{functor}; };
    EXPECT_EQ(functor(), 8);
    EXPECT_THROW(test_call(), std::runtime_error);
}
