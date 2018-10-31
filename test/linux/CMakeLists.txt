CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
PROJECT(atexit)

SET(SRC_LIST atexit.cpp)
add_executable(${PROJECT_NAME} ${SRC_LIST})
add_executable(atof atof.cpp)
add_executable(atoi atoi.cpp)
add_executable(atol atol.cpp)
add_executable(atoll atoll.cpp)
