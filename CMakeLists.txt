cmake_minimum_required(VERSION 3.14)
project(welkin)

##################### build options ########################
option(WELKIN_BUILD_TESTS "build tests" ON)
option(WELKIN_CMAKE_VERBOSE "print verbose cmake messages" ON)
option(WELKIN_SHARED_LIB "shared library support" ON)
option(WELKIN_STDIO "enable standard input and output" ON)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

# set CMAKE_CXX_FLAGS
if(GNU OR Clang)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
endif()

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif()

add_subdirectory(src)