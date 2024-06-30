#!/usr/bin/env sh
# Build the UCG library.
set -xe
mkdir -p lib
cc -c src/ucg.c -o lib/ucg.o -std=c99 -pedantic-errors -Wall -Werror
ar rcs lib/libucg.a lib/ucg.o
ranlib lib/libucg.a
set +xe
