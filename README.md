# sest

simple test framework for C. It's a single header file, just include in your
include path.

## Requirements

You need a fully `C99` compliant compiler, no further dependencies. I
explicitly tested these setups:

- MacOS using `GCC 14` and `Apple Clang 15`
- Linux using `GCC 14` and `Clang 18`

## Features

`sest` has practically no features, and that's by design. You just list tests
functions in a macro, and then it runs those tests sequentially.

Check the example(s) in the `examples` directory for how to use sest.

## Running the examples

If you'd like to run the examples, you can run `make` in this project's root
directory to build them.
The binaries will then be but into `./bin` of the project.

## Custom asserts

`sest` defines a couple of "assert" macros that don't throw an exception when
they fail. If you want your assert to throw, use stdlib's `<assert.h>`. Most of
the time I don't want my test suite to completely stop once an assert fails,
which is why these `sets_assert_*` macros exist.

The macros are defined and documented in the header file `sest.h`.

## LICENSE

> MIT License
>
> Copyright (c) 2024 Tommy Breslein
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
