// you may define this BEFORE including sest.h to disable colored output
// #define __SEST_NO_COLOR__

#include "sest.h"
#include <unistd.h>

// Let's imagine these are two functions we would like to test
size_t slow_factorial(size_t n) {
    sleep(2);
    if (n == 0)
        return 1;
    else
        return (n * slow_factorial(n - 1));
}

size_t factorial(size_t n) {
    if (n == 0)
        return 1;
    else
        return (n * factorial(n - 1));
}

// In order for sest to run your benchmarks, you need to wrap the functions you
// want to bench into a function with an int(void) signature. We will then pass
// these wrapper functions to the sest macros.
int bench_slow_factorial_3(void) { return slow_factorial(3); }
int bench_factorial_100(void) { return factorial(100); }
int bench_factorial_1000(void) { return factorial(1000); }
int bench_factorial_10000(void) { return factorial(10000); }

// Your test executable needs a main function, which should run the
// RUN_SEST_BENCH macro(s).
int main(void) {
    SestBenchConfig conf = {
        // How many warmup runs should sest run. This is by default set to 10.
        // These warmup runs are used to build up a timing estimate to determine
        // how often the actual benchmark runs are ran. sest aims to run the
        // benchmark for the length of the .max_bench_time parameter, or 10
        // seconds if not specified.
        .n_warmup = 10,

        // When determining how often to run each benchmark function, this is
        // the min value. It defaults to 10.
        .min_runs = 10,

        // When determining how often to run each benchmark function, this is
        // the max value. It defaults to 100.
        .max_runs = 100,

        // How long (in seconds) should the whole benchmark run per function.
        .max_bench_time = 10.0,

        // If this is set to a number > 0, it overwrites what sest calculates
        // how often a function should run in a given benchmark.
        .n_runs_fixed = 6,

        // When passing multiple functions to the macro, setting this to > 0
        // enables that sest compares the average runtime of all functions.
        // This is ignored when only passing 1 function.
        .compare = 0,
    };

    // Pass the wrapper functions you want to run to the macro to run your tests
    int err_code = RUN_SEST_BENCH(conf, bench_slow_factorial_3);
    if (err_code > 0) {
        return err_code;
    }

    conf.compare = 1; // enable comparing
    return RUN_SEST_BENCH(conf, bench_factorial_100, bench_factorial_1000,
                          bench_factorial_10000);
}
