cmake_minimum_required(VERSION 3.10)
project(test_kmp)
# �ⲻ��Kmp�㷨�� oh just this 


FILE(GLOB_RECURSE VAR_SRC ${PROJECT_SOURCE_DIR}/../*.cpp)
message("${VAR_SRC}")
add_definitions("-DTEST")
add_executable(${PROJECT_NAME} ${VAR_SRC})

