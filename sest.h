/* sest.h
 * Version: v0.4.0
 *
 * The MIT License (MIT)
 *
 * Copyright © 2024-2025 Tommy Breslein <https://www.github.com/tbreslein>
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef MAX_TEST_NAME_LEN
#define MAX_TEST_NAME_LEN 64
#endif

#ifndef MAX_TEST_NUM
#define MAX_TEST_NUM 1024
#endif

/// helper macros to make the sest_assert_* macros easier to read and write
#define __SEST_ASSERT__(lhs, op, rhs, desc)                                    \
    do {                                                                       \
        if (lhs op rhs) {                                                      \
            __SEST_ASSERT_SUCCESS__(                                           \
                #lhs " " #op " " #rhs, desc, __config, __output);              \
        } else {                                                               \
            __SEST_ASSERT_FAILURE__(#lhs " " #op " " #rhs, __FILE__, __LINE__, \
                desc, __config, __output);                                     \
            if (__config->fail_fast) {                                         \
                return;                                                        \
            }                                                                  \
        }                                                                      \
    } while (0)

#define __SEST_ASSERT_NO_OP__(lhs, op, rhs, desc)                              \
    do {                                                                       \
        if (lhs op rhs) {                                                      \
            __SEST_ASSERT_SUCCESS__(#lhs, desc, __config, __output);           \
        } else {                                                               \
            __SEST_ASSERT_FAILURE__(                                           \
                #lhs, __FILE__, __LINE__, desc, __config, __output);           \
            if (__config->fail_fast) {                                         \
                return;                                                        \
            }                                                                  \
        }                                                                      \
    } while (0)

// === SEST ASSERT MACROS ===
// For most of these, the arguments of the __SEST_ASSERT__ macro tell you what
// the macro tests for
#define sest_assert_eq(lhs, rhs, desc) __SEST_ASSERT__(lhs, ==, rhs, desc)
#define sest_assert_neq(lhs, rhs, desc) __SEST_ASSERT__(lhs, !=, rhs, desc)
#define sest_assert_gt(lhs, rhs, desc) __SEST_ASSERT__(lhs, >, rhs, desc)
#define sest_assert_lt(lhs, rhs, desc) __SEST_ASSERT__(lhs, <, rhs, desc)
#define sest_assert_geq(lhs, rhs, desc) __SEST_ASSERT__(lhs, >=, rhs, desc)
#define sest_assert_leq(lhs, rhs, desc) __SEST_ASSERT__(lhs, <=, rhs, desc)

#define sest_assert_streq(lhs, rhs, desc)                                      \
    do {                                                                       \
        if (strcmp(lhs, rhs) == 0) {                                           \
            __SEST_ASSERT_SUCCESS__(                                           \
                #lhs " == " #rhs, desc, __config, __output);                   \
        } else {                                                               \
            __SEST_ASSERT_FAILURE__(#lhs " == " #rhs, __FILE__, __LINE__,      \
                desc, __config, __output);                                     \
            if (__config->fail_fast) {                                         \
                return;                                                        \
            }                                                                  \
        }                                                                      \
    } while (0)
#define sest_assert_strneq(lhs, rhs, desc)                                     \
    do {                                                                       \
        if (strcmp(lhs, rhs) != 0) {                                           \
            __SEST_ASSERT_SUCCESS__(                                           \
                #lhs " != " #rhs, desc, __config, __output);                   \
        } else {                                                               \
            __SEST_ASSERT_FAILURE__(#lhs " != " #rhs, __FILE__, __LINE__,      \
                desc, __config, __output);                                     \
            if (__config->fail_fast) {                                         \
                return;                                                        \
            }                                                                  \
        }                                                                      \
    } while (0)

// just an assert that you can pass an arbitrary expression to that must
// evaluate to an int or bool
#define sest_assert(cond, desc) __SEST_ASSERT_NO_OP__(cond, !=, 0, desc)

// === SEST IMPLEMENTATION ===
// This is where the "user" stuff ends.

#define color_reset "\033[0m"
#define color_bold_green "\033[1;32m"
#define color_bold_red "\033[1;31m"
#define color_bold_blue "\033[1;34m"
#define color_bold_magenta "\033[1;35m"
#define color_bold_fg "\033[1m"

typedef struct
{
    int fail_fast;
    int no_color;
} __SEST_CONFIG__;

typedef struct
{
    int succ_assertions;
    int fail_assertions;
} __SEST_OUTPUT__;

typedef void __SEST_FN__(const __SEST_CONFIG__ *, __SEST_OUTPUT__ *);
typedef struct
{
    __SEST_OUTPUT__ output;
    char name[MAX_TEST_NAME_LEN];
    __SEST_FN__ *fn;
} __SEST_TEST__;
typedef struct
{
    int len;
    __SEST_TEST__ tests[MAX_TEST_NUM];
} __SEST_TEST_CONTAINER__;

// === SEST PRINTING ===
int __SEST_ASSERT_SUCCESS__(const char *const expr,
    const char *const description, const __SEST_CONFIG__ *c, __SEST_OUTPUT__ *o)
{
    o->succ_assertions++;
    fprintf(stdout, "[ %sSUCCESS%s ]: %s (%s)\n",
        c->no_color == 0 ? "" : color_bold_green,
        c->no_color == 0 ? "" : color_reset, description, expr);
    return 0;
}
int __SEST_ASSERT_FAILURE__(const char *const expr, const char *const file,
    const int line, const char *const description, const __SEST_CONFIG__ *c,
    __SEST_OUTPUT__ *o)
{
    o->fail_assertions++;
    fprintf(stderr, "[ %sFAILURE%s ]: %s (%s) ===> assert location: %s:%d\n",
        c->no_color == 0 ? "" : color_bold_red,
        c->no_color == 0 ? "" : color_reset, description, expr, file, line);
    return 1;
}

#define SEST_CASE(name)                                                        \
    void __sest_case_##name(                                                   \
        const __SEST_CONFIG__ *__config, __SEST_OUTPUT__ *__output);           \
    void __sest_case_proxy_##name(                                             \
        const __SEST_CONFIG__ *__config, __SEST_OUTPUT__ *__output)            \
    {                                                                          \
        __sest_case_##name(__config, __output);                                \
    }                                                                          \
    void __sest_case_##name(                                                   \
        const __SEST_CONFIG__ *__config, __SEST_OUTPUT__ *__output)

#define RUN_SEST_CASE(case)                                                    \
    do {                                                                       \
        if (__sc->len >= MAX_TEST_NUM) {                                       \
            fprintf(stderr,                                                    \
                "Exceeded max number of tests. MAX_TEST_NUM = %d\n",           \
                MAX_TEST_NUM);                                                 \
            exit(1);                                                           \
        }                                                                      \
        __sc->tests[__sc->len++] =                                             \
            (__SEST_TEST__){.name = #case, .fn = __sest_case_proxy_##case};    \
    } while (0);

#define SEST_TEST_MAIN(...)                                                    \
    void __sest_proxy_main__(                                                  \
        const __SEST_CONFIG__ *, __SEST_TEST_CONTAINER__ *__sc);               \
    int main(void)                                                             \
    {                                                                          \
        __SEST_CONFIG__ config = (__SEST_CONFIG__){__VA_ARGS__};               \
        config.no_color = config.no_color == 0 ? isatty(1) : 1;                \
        __SEST_TEST_CONTAINER__ sc = (__SEST_TEST_CONTAINER__){};              \
        __sest_proxy_main__(&config, &sc);                                     \
        printf("\n%s=== STARTING SEST TESTS ===%s\n\n",                        \
            config.no_color == 0 ? "" : color_bold_fg,                         \
            config.no_color == 0 ? "" : color_reset);                          \
        int num_fail_tests = 0;                                                \
        int num_succ_tests = 0;                                                \
        int num_fail_asserts = 0;                                              \
        int num_succ_asserts = 0;                                              \
        for (int i = 0; i < sc.len; i++) {                                     \
            printf("  >>> Test case: %s\n", sc.tests[i].name);                 \
            (sc.tests[i].fn)(&config, &sc.tests[i].output);                    \
            num_succ_asserts += sc.tests[i].output.succ_assertions;            \
            num_succ_tests += sc.tests[i].output.succ_assertions > 0;          \
            num_fail_asserts += sc.tests[i].output.fail_assertions;            \
            num_fail_tests += sc.tests[i].output.fail_assertions > 0;          \
            puts("");                                                          \
        }                                                                      \
        printf("\n%s=== SEST TEST SUMMARY ===%s\n\n",                          \
            config.no_color == 0 ? "" : color_bold_fg,                         \
            config.no_color == 0 ? "" : color_reset);                          \
        printf("%sTests run: %d%s\n",                                          \
            config.no_color == 0 ? "" : color_bold_fg, sc.len,                 \
            config.no_color == 0 ? "" : color_reset);                          \
        printf("%sPassed: %d%s (%d successfull assertions)\n",                 \
            config.no_color == 0 ? "" : color_bold_green, num_succ_tests,      \
            config.no_color == 0 ? "" : color_reset, num_succ_asserts);        \
        printf("%sFailed: %d%s (%d failed assertions)\n",                      \
            config.no_color == 0 ? "" : color_bold_red, num_fail_tests,        \
            config.no_color == 0 ? "" : color_reset, num_fail_asserts);        \
        if (num_fail_tests > 0) {                                              \
            puts("\nThe following tests failed");                              \
            for (int i = 0; i < sc.len; i++) {                                 \
                if (sc.tests[i].output.fail_assertions > 0) {                  \
                    printf("  - %s | %d failed assertions\n",                  \
                        sc.tests[i].name, sc.tests[i].output.fail_assertions); \
                }                                                              \
            }                                                                  \
        }                                                                      \
        return 0;                                                              \
    }                                                                          \
    void __sest_proxy_main__(                                                  \
        const __SEST_CONFIG__ *, __SEST_TEST_CONTAINER__ *__sc)
