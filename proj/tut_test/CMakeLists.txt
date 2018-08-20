cmake_minimum_required(VERSION 3.10)
project(tut_test)
FILE(GLOB_RECURSE SRC ${PROJECT_SOURCE_DIR}/*.cpp)
include_directories(${PROJECT_SOURCE_DIR})
#add_subdirectory(${PROJECT_SOURCE_DIR}/test)
#add_subdirectory(${PROJECT_SOURCE_DIR}/file_test)
message("${SRC}")
add_definitions(-D_MT)
add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} tut)
install(TARGETS ${PROJECT_NAME} DESTINATION bin)


set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/BUILD/)