cmake_minimum_required(VERSION 3.10)
project(test_kmp)
# 这不是Kmp算法； oh just this 


FILE(GLOB_RECURSE VAR_SRC ${PROJECT_SOURCE_DIR}/../*.cpp)
message("${VAR_SRC}")
add_definitions("-DTEST")
add_executable(${PROJECT_NAME} ${VAR_SRC})

