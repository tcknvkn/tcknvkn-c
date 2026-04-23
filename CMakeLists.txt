cmake_minimum_required(VERSION 3.15)
project(tcknvkn C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_library(tcknvkn STATIC src/tcknvkn.c)
target_include_directories(tcknvkn PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(tcknvkn_tests tests/test_tcknvkn.c)
target_link_libraries(tcknvkn_tests PRIVATE tcknvkn)

add_executable(tcknvkn_example examples/quickstart.c)
target_link_libraries(tcknvkn_example PRIVATE tcknvkn)

enable_testing()
add_test(NAME tcknvkn_tests COMMAND tcknvkn_tests)
