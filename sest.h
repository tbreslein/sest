#include <stdio.h>

#pragma once

#define sest_assert_eq(x, y) \
    do { \
        if (x != y) fprintf(stderr, "FAILED! %s != %s\n", #x, #y); \
        else fprintf(stdout, "SUCCESS! %s == %s\n", #x, #y); \
    } while (0);
