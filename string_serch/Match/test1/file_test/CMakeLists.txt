cmake_minimum_required(VERSION 3.10)
project(file_pointer)

set(BOOST_ROOT "D:\\etc\\boost_1_67_0")
  set(Boost_USE_STATIC_LIBS        ON)  # only find static libs
  set(Boost_USE_DEBUG_LIBS         ON) # ignore debug libs and
  set(Boost_USE_RELEASE_LIBS       OFF)  # only find release libs
  set(Boost_USE_MULTITHREADED      ON)
  set(Boost_USE_STATIC_RUNTIME    ON)
find_package(Boost REQUIRED    )

FILE(GLOB_RECURSE SRC_LIST ${PROJECT_SOURCE_DIR}/*.cpp)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
 
	 set(Boost_USE_DEBUG_LIBS ON)
	 #打印一下简单的消息
	 message( STATUS "${Boost_INCLUDE_DIRS};\n${Boost_LIBRARY_DIRS};\n${Boost_LIBRARIES};\n${Boost_VERSION}")
	 message( STATUS "${Boost_LIB_VERSION};\n${Boost_MAJOR_VERSION};\n${Boost_MINOR_VERSION};\n${Boost_LIB_DIAGNOSTIC_DEFINITIONS};\n")
endif()
add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/../BUILD/)