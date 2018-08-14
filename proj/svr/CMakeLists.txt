cmake_minimum_required(VERSION 3.10)
project(svr)
FILE(GLOB_RECURSE SRC ${PROJECT_SOURCE_DIR}/*.cpp)
include_directories(${PROJECT_SOURCE_DIR})
#add_subdirectory(${PROJECT_SOURCE_DIR}/test)
#add_subdirectory(${PROJECT_SOURCE_DIR}/file_test)
message("${SRC}")
set(BOOST_ROOT "D:\\etc\\boost_1_67_0")
set(BOOST_INCLUDEDIR ${BOOST_ROOT}\\)
set(BOOST_LIBRARYDIR ${BOOST_ROOT}\\stage)
set(Boost_NO_SYSTEM_PATHS  ON)
 
 set(Boost_USE_DEBUG_LIBS			ON) 
set(Boost_USE_STATIC_LIBS			ON) # only find static libs
set(Boost_USE_MULTITHREADED			ON)
#set(Boost_USE_DEBUG_LIBS			ON) # ignore debug libs and
set(Boost_USE_STATIC_RUNTIME		OFF)
#set(Boost_USE_RELEASE_LIBS			ON) 
add_definitions(-D_MT)
find_package(Boost 1.67.0  COMPONENTS system  regex date_time  )

 
if(Boost_FOUND)
    
 
	 
	 #打印一下简单的消息
	 #message( STATUS "${Boost_INCLUDE_DIRS};\n${Boost_LIBRARY_DIRS};\n${Boost_LIBRARIES};\n${Boost_VERSION}")
	 #message( STATUS "${Boost_LIB_VERSION};\n${Boost_MAJOR_VERSION};\n${Boost_MINOR_VERSION};\n${Boost_LIB_DIAGNOSTIC_DEFINITIONS};\n")
	 message(STATUS "${Boost_FIND_COMPONENTS}")
endif()

include_directories(${Boost_INCLUDE_DIRS})
add_executable(${PROJECT_NAME}  ${SRC})
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} )
set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/BUILD/)