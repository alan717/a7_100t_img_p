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
# 
set(MYTEST_VERSION_MAJOR 2)
set(MYTEST_VERSION_MINOR 21)
set(MYTEST_VERSION_DATABASE 126)
set(MYTEST_VERSION_SOURCES_FILE 14)
set(MYTEST_VERSION ${MYTEST_VERSION_MAJOR}.${MYTEST_VERSION_MINOR}.${MYTEST_VERSION_DATABASE})
if (MYTEST_ENABLE_DEV_OPTION)
	set(CPACK_PACKAGE_INSTALL_DIRECTORY "RTAGS")
    set(CPACK_PACKAGE_VENDOR "RTags")
    set(CPACK_PACKAGE_CONTACT "http://rtags.net")
    set(CPACK_PACKAGE_VERSION_MAJOR ${MYTEST_VERSION_MAJOR})
    set(CPACK_PACKAGE_VERSION_MINOR ${RTAGS_VERSION_MINOR})
    set(CPACK_PACKAGE_VERSION_DATABASE ${RTAGS_VERSION_DATABASE})
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")
    set(CPACK_PACKAGE_VERSION "${MYTEST_VERSION}")
	if (NOT CPACK_GENERATOR)
        set(CPACK_GENERATOR TGZ)
    endif ()
	set(CPACK_SOURCE_PACKAGE_FILE_NAME "rtags-${RTAGS_VERSION_MAJOR}.${RTAGS_VERSION_MINOR}"
        CACHE INTERNAL "tarball basename")
		set(CPACK_SOURCE_GENERATOR ${CPACK_GENERATOR})
	set(CPACK_SOURCE_IGNORE_FILES
        "~$"
        "\\\\.gitignore$"
        "\\\\.gitmodules$"
        "\\\\.git/"
        "\\\\.travis.yml$"
        "\\\\.github/"
        "^${PROJECT_SOURCE_DIR}/.dir-locals.el"
        "^${PROJECT_SOURCE_DIR}/build/"
        )
	include(CPack)
else()
	set(RCT_WITH_TESTS ${BUILD_TESTING})
	set(CMAKE_LEGACY_CYGWIN_WIN32 0)
    if (APPLE)
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g")
    else ()
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ggdb")
    endif ()
	#ASAN是clang内建的一种检查内存泄露的工具,主要是clang或者是gcc4.8
    if (ASAN)
        string(REPLACE "," ";" ASANS ${ASAN})
        foreach (SANITIZE ${ASANS})
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=${SANITIZE}")
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=${SANITIZE}")
        endforeach ()
    endif ()
   if (NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Release CACHE STRING
            "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
    endif()
    #
	if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
        set(CMAKE_COMPILER_IS_CLANGCXX TRUE)
    endif()
	#
	set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
	include(FeatureSummary)
	# RPATH   -->LD_LIBRARY_PATH -->RUNPATH(出现会屏蔽RPATH) -->/etc/ld.so.conf -->builtin dircetories(/lib, /usr/lib)
	if (EXISTS "rules.ninja")
        add_custom_target(release COMMAND ${CMAKE_COMMAND} -GNinja -DCMAKE_BUILD_TYPE=Release . WORKING_DIRECTORY .)
        add_custom_target(debug COMMAND ${CMAKE_COMMAND} -GNinja -DCMAKE_BUILD_TYPE=Debug . WORKING_DIRECTORY .)
    else()
        add_custom_target(release COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Release . WORKING_DIRECTORY .)
        add_custom_target(debug COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Debug . WORKING_DIRECTORY .)
    endif()
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
add_subdirectory(bfs)
add_subdirectory(dfs)
add_subdirectory(dijkstra)
add_subdirectory(boost_test)
add_subdirectory(boost_svr)
add_subdirectory(boost_cli)
add_subdirectory(time_sync)
add_subdirectory(reflects)

