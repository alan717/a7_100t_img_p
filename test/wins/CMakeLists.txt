CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
PROJECT(wins)

SET(SRC_LIST main.cpp)
include_directories(${PROJECT_SOURCE_DIR})
add_executable(${PROJECT_NAME} ${SRC_LIST})

