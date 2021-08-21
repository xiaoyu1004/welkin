# --- CPU files
set(WELKIN_SRCS)

# --- add source file
add_subdirectory(core)
list(APPEND WELKIN_SRCS ${WELKIN_CPU_SRCS})
add_subdirectory(operators)
list(APPEND WELKIN_SRCS ${WELKIN_CPU_SRCS})
add_subdirectory(utils)
list(APPEND WELKIN_SRCS ${WELKIN_CPU_SRCS})

if(WELKIN_BUILD_TESTS)
    add_subdirectory(test)
endif()

# get a list of source files
message(STATUS "CPU sources: ")
foreach(tmp ${WELKIN_SRCS})
message(STATUS "  " ${tmp})
endforeach()

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
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/core>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/core>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/operators>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/utils>)

# # Threads support
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)
target_link_libraries(welkin PUBLIC Threads::Threads)