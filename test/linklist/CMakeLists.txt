CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
PROJECT(link_list)
include_directories(${PROJECT_SOURCE_DIR}/inc/)
add_executable(complex_num_test src/main.cpp)
add_executable(link_tst src/test.cpp)
