cmake_minimum_required(VERSION 2.8)

project("sb-linker")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

set(PROJECT_C_FLAGS "-std=c99 -Wall -Wextra -Werror -g -pg")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${PROJECT_C_FLAGS}")

enable_testing()

add_subdirectory(src)

add_subdirectory(tst/unit-tests)
