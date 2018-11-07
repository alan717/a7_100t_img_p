CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
PROJECT(test)
get_directory_property(has_parent PARENT_DIRECTORY)
if(NOT has_parent)
    # When you call add_subdirectory(dlib) from a parent CMake project dlib's
    # CMake scripts will assume you want to statically compile dlib into
    # whatever you are building rather than create a standalone copy of dlib.
    # This means CMake will build dlib as a static library, disable dlib's
    # install targets so they don't clutter your project, and adjust a few other
    # minor things that are convenient when statically building dlib as part of
    # your own projects.
    #
    # On the other hand, if there is no parent CMake project or if
    # DLIB_IN_PROJECT_BUILD is set to false, CMake will compile dlib as a normal
    # standalone library (either shared or static, based on the state of CMake's
    # BUILD_SHARED_LIBS flag), and include the usual install targets so you can
    # install dlib on your computer via `make install`.  Since the only reason
    # to build this CMakeLists.txt (the one you are reading right now) by itself
    # is if you want to install dlib, we indicate as such by setting
    # DLIB_IN_PROJECT_BUILD to false.
    set(DLIB_IN_PROJECT_BUILD false)
    message(STATUS "not have parent folder")
endif()

message("Checking CMAKE_SYSTEM_NAME = '${CMAKE_SYSTEM_NAME}'")
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    add_definitions(-DOS_OSX)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    add_definitions(-DOS_LINUX)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    add_definitions(-DOS_WIN)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Android")
    add_definitions(-DOS_ANDROID)
    message("Checking CMAKE_ABI_NAME = '${CMAKE_ANDROID_ARCH_ABI}'")
endif()
set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/build/)

add_subdirectory(queue)
add_subdirectory(disptcher)
add_subdirectory(linux)
add_subdirectory(wins)
add_subdirectory(algorithms)
add_subdirectory(opencv_arch)
add_subdirectory(linklist)
