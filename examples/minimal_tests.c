// you may define this BEFORE including sest.h to disable colored output
// #define __SEST_NO_COLOR__

#include "sest.h"

// Let's imagine this is the function we would like to test
static int add(int x, int y) { return x + y; }

// These are a couple of example test functions
// The only important pieces when you write test functions are:
//   - they must not take any arguments
//   - they must return an int indicating failure if the return value is > 0

// This test will simply succeed, and thus be listed as a successful test
static int foo(void)
{
    int errcode = 0;
    errcode += sest_assert_eq(2, add(1, 1), "1 + 1 == 2");
    return errcode;
}

// One of these asserts will fail, so this test function will be listed as
// failed
static int bar(void)
{
    int errcode = 0;
    errcode += sest_assert_gt(2, add(0, 1), "0 + 1 < 2");
    errcode += sest_assert_gt(1, add(0, 1), "1 + 1 < 2 (will fail)");
    return errcode;
}

// This one's going to fail too
static int foobar(void)
{
    int errcode = 0;
    errcode += sest_assert_neq(2, add(1, 1), "1 + 1 != 2 (will fail)");
    return errcode;
}

// Your test executable needs a main function, which should run the
// RUN_SEST_TEST macro.
int main(void)
{
    // Pass the test functions you want to run to the macro to run your tests
    return RUN_SEST_TEST(foo, foobar, bar);
}
