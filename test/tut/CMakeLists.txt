cmake_minimum_required(VERSION 3.10)
project(tut)
FILE(GLOB_RECURSE SRC ${PROJECT_SOURCE_DIR}/*.h ${PROJECT_SOURCE_DIR}/tut/*.hpp)
include_directories(${PROJECT_SOURCE_DIR})
#add_subdirectory(${PROJECT_SOURCE_DIR}/test)
#add_subdirectory(${PROJECT_SOURCE_DIR}/file_test)
message("${SRC}")
 
 
 
add_definitions(-D_MT)
 

 
add_library(${PROJECT_NAME} INTERFACE )
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
target_include_directories(${PROJECT_NAME} INTERFACE ${PROJECT_SOURCE_DIR}/tut/ ${PROJECT_SOURCE_DIR}/)

set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/BUILD/)