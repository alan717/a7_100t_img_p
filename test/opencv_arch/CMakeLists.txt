CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
PROJECT(opencv_arch)

 
set("OpenCV_DIR" "D:\\cur\\opencv\\build\\x64\\vc14\\lib")
 set(OpenCV_STATIC ON)
find_package(OpenCV REQUIRED)
 
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(DisplayImage DisplayImage.cpp)
target_link_libraries(DisplayImage ${OpenCV_LIBS})
message( "HOST:'${CMAKE_STATIC_LINKER_FLAGS}',win64:'${WIN64}',exe:'${CMAKE_EXE_LINKER_FLAGS}',runtime:'${OpenCV_RUNTIME}'")
 
