/* sest.h
 * Version: v0.1.1
 *
 * The MIT License (MIT)
 *
 * Copyright © 2024 Tommy Breslein <https://www.github.com/tbreslein>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#pragma once

#include <stddef.h>
/*#include <stdint.h>*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
/*#include <sys/times.h>*/

/// custom fprintf(stdout, ...) function to indicate a successful test
int __SEST_SUCCESS__(const char *const expr);

/// custom fprintf(stderr, ...) function to indicate a failed test
int __SEST_FAILURE__(const char *const expr, const char *const file,
                     const int line);

/// helper macros to make the sest_assert_* macros easier to read and write
#define __SEST_ASSERT__(lhs, op, rhs)                                          \
    (lhs op rhs) ? __SEST_SUCCESS__(#lhs " " #op " " #rhs)                     \
                 : __SEST_FAILURE__(#lhs " " #op " " #rhs, __FILE__, __LINE__)

// === SEST ASSERT MACROS ===
// For most of these, the arguments of the __SEST_ASSERT__ macro tell you what
// the macro tests for
#define sest_assert_eq(lhs, rhs) __SEST_ASSERT__(lhs, ==, rhs)
#define sest_assert_neq(lhs, rhs) __SEST_ASSERT__(lhs, !=, rhs)
#define sest_assert_gt(lhs, rhs) __SEST_ASSERT__(lhs, >, rhs)
#define sest_assert_lt(lhs, rhs) __SEST_ASSERT__(lhs, <, rhs)
#define sest_assert_geq(lhs, rhs) __SEST_ASSERT__(lhs, >=, rhs)
#define sest_assert_leq(lhs, rhs) __SEST_ASSERT__(lhs, <=, rhs)

// just an assert that you can pass an arbitrary expression to that must
// evaluate to an int or bool
#define sest_assert(cond) __SEST_ASSERT__(cond, ==, 0)

// === SEST IMPLEMENTATION ===
// This is where the "user" stuff ends.

#ifndef __SEST_NO_COLOR__
static const char *const color_reset = "\033[0m";
static const char *const color_bold_green = "\033[1;32m";
static const char *const color_bold_red = "\033[1;31m";
static const char *const color_bold_fg = "\033[1m";
#else
static const char *const color_reset = "";
static const char *const color_bold_green = "";
static const char *const color_bold_red = "";
static const char *const color_bold_fg = "";
#endif

// === SEST PRINTING ===
int __SEST_SUCCESS__(const char *const expr) {
    fprintf(stdout, "[ %sSUCCESS%s ]: %s\n", color_bold_green, color_reset,
            expr);
    return 0;
}
int __SEST_FAILURE__(const char *const expr, const char *const file,
                     const int line) {
    fprintf(stderr, "[ %sFAILURE%s ]: %s ===> assert location: %s:%d\n",
            color_bold_red, color_reset, expr, file, line);
    return 1;
}

// === SEST UTILIY ===
static size_t __sest_get_num_names(const char *const s) {
    size_t count = 1;
    const size_t len = strlen(s);
    if (len == 0) {
        return 0;
    }
    for (size_t i = 0; i < len; i++)
        count += s[i] == ',' ? 1 : 0;
    return count;
}

static char **__get_sest_func_name_array(char *test_names,
                                         const size_t num_names) {
    const size_t name_max_len = 128;
    char **name_array = malloc(num_names * sizeof(char *));

    if (!name_array) {
        fprintf(stderr, "malloc error at %s:%d\n", __FILE__, __LINE__);
        exit(1);
    }

    for (size_t i = 0; i < num_names; i++) {
        name_array[i] = malloc(name_max_len * sizeof(char));
        if (!name_array[i]) {
            fprintf(stderr, "malloc error at %s:%d\n", __FILE__, __LINE__);
            exit(1);
        }
    }

    const size_t k_max = strlen(test_names);
    for (size_t j = 0, k = 0; j < num_names; j++) {
        for (size_t i = 0; i < name_max_len; i++, k++) {
            if (k == k_max) {
                name_array[j][i] = '\0';
                break;
            } else if (test_names[k] == ',') {
                name_array[j][i] = '\0';
                k += 2; // skip over the , AND the space
                break;
            } else {
                name_array[j][i] = test_names[k];
            }
        }
    }

    return name_array;
}

// === SEST TEST STUFF ===
typedef int SestTest(void);
int __run_sest_tests(SestTest **tests, char *test_name_string);

int __run_sest_tests(SestTest **tests, char *test_name_string) {
    printf("\n\n%s=== STARTING SEST TESTS ===%s\n", color_bold_fg, color_reset);
    const size_t num_tests = __sest_get_num_names(test_name_string);
    int num_err = 0;
    int num_success = num_tests;
    char **test_name_array =
        __get_sest_func_name_array(test_name_string, num_tests);

    int *err_codes = malloc(num_tests * sizeof(int));
    if (!err_codes) {
        fprintf(stderr, "malloc error at %s:%d\n", __FILE__, __LINE__);
        exit(1);
    }

    for (size_t i = 0; i < num_tests; i++) {
        printf("\n%sRunning test: %s%s\n", color_bold_fg, test_name_array[i],
               color_reset);
        err_codes[i] = (tests[i])();
        num_err += err_codes[i] != 0;
        num_success -= err_codes[i] != 0;
    }

    printf("\n\n%s=== SEST TEST SUMMARY ===%s\n", color_bold_fg, color_reset);
    printf("%sTests run: %zu%s\n", color_bold_fg, num_tests, color_reset);
    printf("%sPassed: %d%s\n", color_bold_green, num_success, color_reset);
    printf("%sFailed: %d%s\n", color_bold_red, num_err, color_reset);
    if (num_err > 0) {
        printf("\nThe following tests failed:\n");
        for (size_t i = 0; i < num_tests; i++) {
            if (err_codes[i] > 0) {
                printf("  - %s | error code: %d\n", test_name_array[i],
                       err_codes[i]);
            }
        }
    }

    for (size_t i = 0; i < num_tests; i++) {
        free(test_name_array[i]);
    }
    free(test_name_array);
    free(err_codes);

    return num_err;
}

#define RUN_SEST_TEST(...)                                                     \
    __run_sest_tests((SestTest *[]){__VA_ARGS__, NULL}, #__VA_ARGS__)

// === SEST BENCH STUFF ===
typedef int SestBench(void);
int __run_sest_bench(SestBench **benches, char *bench_name_string);

int __run_sest_bench(SestBench **benches, char *bench_name_string) {
    printf("\n\n%s=== STARTING SEST BENCHES ===%s\n", color_bold_fg,
           color_reset);
    const size_t num_benches = __sest_get_num_names(bench_name_string);
    int num_err = 0;
    char **bench_name_array =
        __get_sest_func_name_array(bench_name_string, num_benches);

    int *err_codes = malloc(num_benches * sizeof(int));
    if (!err_codes) {
        fprintf(stderr, "malloc error at %s:%d\n", __FILE__, __LINE__);
        exit(1);
    }

    const size_t n_runs = 1000;
    int *run_timings = malloc(n_runs * sizeof(int));
    if (!run_timings) {
        fprintf(stderr, "malloc error at %s:%d\n", __FILE__, __LINE__);
        exit(1);
    }

    struct timespec start, end;

    for (size_t i_bench = 0; i_bench < num_benches; i_bench++) {
        printf("\n%sRunning bench: %s%s\n", color_bold_fg,
               bench_name_array[i_bench], color_reset);

        int sum_timings = 0;
        int num_failed_bench_runs = 0;
        for (size_t i_run = 0; i_run < n_runs; i_run++) {
            int x;
            clock_gettime(CLOCK_REALTIME, &start);
            x = (benches[i_bench])() > 0;
            clock_gettime(CLOCK_REALTIME, &end);
            num_failed_bench_runs += x;

            run_timings[i_run] = end.tv_nsec - start.tv_nsec;
            sum_timings += run_timings[i_run];
        }
        err_codes[i_bench] = num_failed_bench_runs;
        double avg_timing = (double)sum_timings / (double)n_runs;
        printf("Average Timing: %e ns\n\n", avg_timing);

        num_err += num_failed_bench_runs > 0;
    }

    for (size_t i = 0; i < num_benches; i++) {
        free(bench_name_array[i]);
    }
    free(bench_name_array);
    free(err_codes);

    return num_err;
}

#define RUN_SEST_BENCH(...)                                                    \
    __run_sest_bench((SestBench *[]){__VA_ARGS__, NULL}, #__VA_ARGS__)
