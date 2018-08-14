cmake_minimum_required(VERSION 3.10)
project(BMH)
FILE(GLOB_RECURSE VAR ${PROJECT_SOURCE_DIR}/*.cpp)
add_executable(${PROJECT_NAME} ${VAR})

