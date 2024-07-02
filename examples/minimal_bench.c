// you may define this BEFORE including sest.h to disable colored output
// #define __SEST_NO_COLOR__

#include "sest.h"
#include <unistd.h>

// Let's imagine this is the function we would like to test
size_t factorial(size_t n) {
    sleep(1);
    if (n == 0)
        return 1;
    else
        return (n * factorial(n - 1));
}

int bench_factorial_3(void) { return factorial(3); }
int bench_factorial_100(void) { return factorial(100); }
int bench_factorial_1000(void) { return factorial(1000); }
int bench_factorial_10000(void) { return factorial(10000); }

// Your test executable needs a main function, which should run the
// RUN_SEST_TEST macro.
int main(void) {
    // Pass the test functions you want to run to the macro to run your tests
    /*return RUN_SEST_BENCH(bench_factorial_100, bench_factorial_1000,*/
    /*                      bench_factorial_10000);*/
    return RUN_SEST_BENCH(bench_factorial_3);
}
