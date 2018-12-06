cmake_minimum_required(VERSION 3.10)
project(boost_chaoshijiance)
FILE(GLOB_RECURSE SRC ${PROJECT_SOURCE_DIR}/*.cpp)
include_directories(${PROJECT_SOURCE_DIR})
#add_subdirectory(${PROJECT_SOURCE_DIR}/test)
#add_subdirectory(${PROJECT_SOURCE_DIR}/file_test)
message("${SRC}")
message(WARNING "fuck ${PROJECT_NAME}")
#set(Boost_DIR "D:\\etc\\boost_1_67_0")
set(BOOST_ROOT "D:\\repo\\boost_1_68_0")

#set(BOOST_INCLUDEDIR ${BOOST_ROOT}\\)
#set(BOOST_LIBRARYDIR ${BOOST_ROOT}\\stage\\lib)
#Boost_LIBRARY_DIRS
#set(Boost_NO_SYSTEM_PATHS  ON)
 
set(Boost_USE_DEBUG_LIBS			ON) 
set(Boost_USE_STATIC_LIBS			ON) # only find static libs
set(Boost_USE_MULTITHREADED			ON)
#set(Boost_USE_DEBUG_LIBS			ON) # ignore debug libs and
set(Boost_USE_STATIC_RUNTIME		OFF)
#set(Boost_USE_RELEASE_LIBS			ON) 
add_definitions(-D_MT )
find_package(Boost  1.67.0  REQUIRED  COMPONENTS system  regex date_time thread)

 
if(Boost_FOUND)
     #打印一下简单的消息  库:${Boost_LIBRARIES};\n
	 message( STATUS "包含:${Boost_INCLUDE_DIRS};\n库目录:${Boost_LIBRARY_DIRS};\n版本:${Boost_VERSION}\n参数:${Boost_LIB_DIAGNOSTIC_DEFINITIONS}")
	 #message( STATUS "${Boost_LIB_VERSION};\n${Boost_MAJOR_VERSION};\n${Boost_MINOR_VERSION};\n${Boost_LIB_DIAGNOSTIC_DEFINITIONS};\n")
	  
	 message( WARNING "组建:${Boost_FIND_COMPONENTS}${PROJECT_NAME}")
	 else()
	 message(WARNING "not found boost")
endif()

include_directories(${Boost_INCLUDE_DIRS})
add_executable(${PROJECT_NAME}  ${SRC})
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} )
link_directories(${Boost_LIBRARIES}) #这条命令主要是传相对路径进行链接.
#但是target_link_libraries已经将找到的绝对路径链接了.所以很少使用
#set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/BUILD/)