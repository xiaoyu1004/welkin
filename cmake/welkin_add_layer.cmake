macro(welkin_add_layer class)
    string(TOLOWER ${class} name)

    if(${ARGC} EQUAL 2)
        option(WITH_LAYER_${name} "build with layer ${name}" ${ARGV1})
    elseif()
        option(WITH_LAYER_${name} "build with layer ${name}" ON)
    endif()

    if(WELKIN_CMAKE_VERBOSE)
        message(STATUS "WITH_LAYER_${name} = ${WITH_LAYER_${name}}")
    endif()

    if(WITH_LAYER_${name})
        list(APPEND WELKIN_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/layer/${name}.cpp)
    endif()
endmacro()