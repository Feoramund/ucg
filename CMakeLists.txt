cmake_minimum_required(VERSION 3.15...3.29)

project(
  UCG
  VERSION 0.1
  LANGUAGES C)

include(CTest)

if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

add_library(ucg src/ucg.c src/ucg.h)

add_executable(test_runner tests/test_runner.c)

target_link_libraries(test_runner PRIVATE ucg)
add_test(NAME All_Tests COMMAND test_runner)
