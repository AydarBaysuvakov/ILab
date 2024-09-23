cmake_minimum_required(VERSION 3.11)
project("cache_project")

set(SOURCE cache.cpp)
set(IDEAL  ideal.cpp)
set(TESTS  tests.cpp)

add_executable(2q_cache    ${SOURCE})
add_executable(ideal_cache ${IDEAL})
add_executable(tests       ${TESTS})