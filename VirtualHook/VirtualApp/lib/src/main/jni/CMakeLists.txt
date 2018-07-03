cmake_minimum_required(VERSION 3.4.1)

project(va++)
file(GLOB SRC ${CMAKE_SOURCE_DIR}/Jni/*.cpp )
message(STATUS "${SRC}")
