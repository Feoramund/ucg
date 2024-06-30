#!/usr/bin/env sh
# Build and run the UCG tests.
set -xe
cc test_runner.c -o test_runner -L../lib -lucg -std=c99 -pedantic-errors -Wall -Werror
./test_runner
set +xe
