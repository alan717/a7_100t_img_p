# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library(pictool SHARED main.cpp ClickHelper.cpp define.cpp)

add_library(pictool_a STATIC main.cpp ClickHelper.cpp define.cpp)

aux_source_directory(./lua LUA_SRC_LIST)
aux_source_directory(./Substrate SUBSTRATE_SRC_LIST)

add_library(lua STATIC ${LUA_SRC_LIST})



# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library(log-lib log )

add_library(bot SHARED bot/SymbolFinder.cpp bot/bot.cpp Substrate/hde64.c Substrate/SubstrateDebug.cpp Substrate/SubstrateHook.cpp Substrate/SubstratePosixMemory.cpp)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

add_library(libopencv SHARED IMPORTED)
set_target_properties(libopencv PROPERTIES IMPORTED_LOCATION
    ${CMAKE_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libopencv_java3.so)

add_library(curl_a STATIC IMPORTED)
set_target_properties(curl_a PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libcurl.a)
target_link_libraries(pictool ${log-lib} libopencv)
target_link_libraries(bot ${log-lib})


include_directories(${CMAKE_SOURCE_DIR}/opencv/include)
include_directories(${CMAKE_SOURCE_DIR}/lua)
include_directories(${CMAKE_SOURCE_DIR}/Substrate)
include_directories(${CMAKE_SOURCE_DIR}/)
include_directories(${CMAKE_SOURCE_DIR}/lua)



set(SRC_LIST ${CMAKE_SOURCE_DIR}/lua_logic.cpp
        ${CMAKE_SOURCE_DIR}/luaImportList.cpp
        ${CMAKE_SOURCE_DIR}/http.cpp)
add_executable(main_logic    ${SRC_LIST})
target_link_libraries(pictool_a libopencv ${log-lib})
target_link_libraries(main_logic lua    curl_a log z )
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIE")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIE -llog -pie -frtti -fexceptions -std=c++11 -lz ")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/../build/${ANDROID_ABI}/")