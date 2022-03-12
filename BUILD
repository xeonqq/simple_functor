cc_library(
    name = "simple_functor",
    hdrs = ["include/simple_functor.h"],
    includes = ["include"],
    deps = [
    ],
)

cc_test(
    name = "simple_functor_test",
    srcs = glob(["test/simple_functor_test.cpp"]),
    deps = [
        ":simple_functor",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)
