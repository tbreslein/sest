# sest

A simple single header test framework for C.

## Requirements

You need a `C99` compliant compiler, no further dependencies. I explicitly
tested these setups:

- MacOS using `GCC 14` and `Apple Clang 15`
- Linux using `GCC 14` and `Clang 18`

## Features

`sest` has practically no features, and that's by design. You just define a
couple of tests, and then list which of these you want to run in a different
macro.

For any such test case, `sest` provides a bunch of custom assert macros you can
use. Take a look at the example(s) in the repository for how to structure a
simple test program.

## Configuration

The `SEST_TEST_MAIN` macro takes a variadic amount of arguments. Those arguments
are the struct fields of the underlying `__SEST_CONFIG__` struct. Look up the
struct definition in `sest.h` for docs.

For example, by default, `sest` produces colored output. If you don't want that,
add the `.no_color = 1` in the macro arguments like this:

```c
SEST_TEST_MAIN(.no_color = 1)
```

## Running the examples

If you'd like to run the examples, you can run `make` in this project's root
directory to build them. The binaries will then be but into `./bin` of the
project.

## Custom asserts

`sest` defines a couple of "assert" macros that don't throw an exception when
they fail. If you want your assert to throw, use stdlib's `<assert.h>`. Most of
the time I don't want my test suite to completely stop once an assert fails,
which is why these `sets_assert_*` macros exist.

Note that these macros have a return value! They return `1` in case the assert
failed, and `0` if it passed. Use this to keep track of the number of failed
asserts in your test.

The macros are defined and documented in the header file `sest.h`.

## LICENSE

> MIT License
>
> Copyright (c) 2024-2025 Tommy Breslein
>
> Permission is hereby granted, free of charge, to any person obtaining a copy
> of this software and associated documentation files (the "Software"), to deal
> in the Software without restriction, including without limitation the rights
> to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
> copies of the Software, and to permit persons to whom the Software is
> furnished to do so, subject to the following conditions:
>
> The above copyright notice and this permission notice shall be included in all
> copies or substantial portions of the Software.
>
> THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
> IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
> FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
> AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
> LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
> OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
> SOFTWARE.
