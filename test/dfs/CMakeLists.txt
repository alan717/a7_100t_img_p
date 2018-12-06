cmake_minimum_required(VERSION 3.10)
project(dfs)
FILE(GLOB_RECURSE SRC ${PROJECT_SOURCE_DIR}/*.cpp)
include_directories(${PROJECT_SOURCE_DIR})
#add_subdirectory(${PROJECT_SOURCE_DIR}/test)
#add_subdirectory(${PROJECT_SOURCE_DIR}/file_test)
message("${SRC}")
 
 
 
add_definitions(-D_MT)
 

 
add_executable(dfs dfs.cpp)
install(TARGETS ${PROJECT_NAME} DESTINATION bin)


#set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/BUILD/)