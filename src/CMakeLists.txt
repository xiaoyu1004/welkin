# aim to adapt difference platform
configure_file(platform.h.in ${CMAKE_CURRENT_BINARY_DIR}/platform.h)

# collect source file
set(WELKIN_SRCS
    tensor.cpp
)

# include welkin_add_layer.cmake
include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/welkin_add_layer.cmake)

# layer implementation
welkin_add_layer(innerproduct)
welkin_add_layer(sigmoid)

# build library
if(WELKIN_SHARED_LIB)
    add_library(welkin SHARED ${WELKIN_SRCS})
elseif()
    add_library(welkin STATIC ${WELKIN_SRCS})
endif()

# export interface
include(GenerateExportHeader)
generate_export_header(welkin)

# include directories
target_include_directories(welkin
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/layer>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/common>)

# Threads support
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)
target_link_libraries(welkin PUBLIC Threads::Threads)
