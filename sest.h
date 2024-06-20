#pragma once

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int __SEST_SUCCESS__(const char *const expr) {
    fprintf(stdout, "[ \033[1;32mSUCCESS\033[0m ]: %s\n", expr);
    return 0;
}
int __SEST_FAILURE__(const char *const expr, const char *const file,
                     const int line) {
    fprintf(stderr,
            "[ \033[1;31mFAILURE\033[0m ]: %s ===> assert location: %s:%d\n",
            expr, file, line);
    return 1;
}

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

static char **__get_sest_test_name_array(char *test_names,
                                         const size_t num_names) {
    const size_t name_max_len = 128;
    char **name_array = malloc(num_names * sizeof(char *));

    if (!name_array) {
        fprintf(stderr, "malloc error at %s:%d", __FILE__, __LINE__);
        exit(1);
    }

    for (size_t i = 0; i < num_names; i++) {
        name_array[i] = NULL;
        name_array[i] = malloc(name_max_len * sizeof(char));
        if (!name_array[i]) {
            fprintf(stderr, "malloc error at %s:%d", __FILE__, __LINE__);
            exit(1);
        }
    }

    for (size_t j = 0, k = 0; j < num_names; j++) {
        for (size_t i = 0; i < name_max_len; i++, k++) {
            if (test_names[k] == ',') {
                k += 2; // skip over the , AND the space
                break;
            } else {
                name_array[j][i] = test_names[k];
            }
        }
    }

    return name_array;
}

typedef int SestTest(void);

int __run_sest_tests(SestTest **tests, char *test_name_string);

int __run_sest_tests(SestTest **tests, char *test_name_string) {
    const size_t num_tests = __sest_get_num_names(test_name_string);
    int num_err = 0;
    int num_success = num_tests;
    char **test_name_array =
        __get_sest_test_name_array(test_name_string, num_tests);

    int *err_codes = malloc(num_tests * sizeof(int));

    if (!err_codes) {
        fprintf(stderr, "malloc error at %s:%d", __FILE__, __LINE__);
        exit(1);
    }

    for (size_t i = 0; i < num_tests; i++) {
        printf("\n\033[1mRunning test: %s\033[0m\n", test_name_array[i]);
        err_codes[i] = (tests[i])();
        num_err += err_codes[i] != 0;
        num_success -= err_codes[i] != 0;
    }

    printf("\n\n\033[1m=== SEST SUMMARY ===\033[0m\n");
    printf("\033[1mTests run: %zu\033[0m\n", num_tests);
    printf("\033[1;32mPassed: %d\033[0m\n", num_success);
    printf("\033[1;31mFailed: %d\033[0m\n", num_err);
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
