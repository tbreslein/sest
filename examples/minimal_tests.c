// you may define this BEFORE including sest.h to disable colored output
// #define __SEST_NO_COLOR__

#include "sest.h"

// These are a couple of example test functions
// The only important pieces when you write test functions are:
//   - they must not take any arguments
//   - they must return an int indicating failure if the return value is > 0

// Let's imagine this is the function we would like to test
static int add(int x, int y) { return x + y; }

// This macro defines a test case. The argument for this case needs to be a
// valid function name.
// This test will simply succeed, and thus be listed as a successful test
SEST_CASE(add_success) { sest_assert_eq(add(1, 1), 2, "1 + 1 == 2"); }

// One of these asserts will fail, so this test function will be listed as
// failed
SEST_CASE(add_fail)
{
    sest_assert_gt(add(0, 1), 2, "0 + 1 > 2");
    sest_assert_gt(add(2, 1), 2, "2 + 1 > 2");
}

// This one's going to fail too
SEST_CASE(foobar)
{
    sest_assert_streq("foo", "foo", "\"foo\" == \"foo\"");
    sest_assert_strneq("bar", "bar", "\"bar\" != \"bar\"");
}

// Your test executable needs a main function. You can write your own, but you
// should probably just use this macro which defines a main function and a bunch
// of boilerplate code to make sest work.
// Inside of this function, you can register the test cases you want to run with
// the RUN_SEST_CASE macros. You may also pass arguments to this function to
// configure the tests, as if the argument list was the init list of the
// __SEST_CONFIG__ struct defined in sest.h. At the time of writing this, these
// are the things you can configure (all default values are 0):
//
// - .fail_fast: set this to 1 in order for any test case to return immediately
//     upon encountering a failed assertion
// - .no_color: set this to 1 to suppress any color output
//
// For example, if you wanted to enable both of these options, you would call
// the like this:
// SEST_TEST_MAIN(.fail_fast = 1, .no_color = 1)
// or if you just wanted to enable fail_fast, then invoke the macro like this:
// SEST_TEST_MAIN(.fail_fast = 1)
SEST_TEST_MAIN()
{
    // register this sest case to run. argument must match the name of a sest
    // case.
    RUN_SEST_CASE(add_success);
    RUN_SEST_CASE(add_fail);
    RUN_SEST_CASE(foobar);
}
