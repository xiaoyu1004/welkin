include_directories(${PROJECT_SOURCE_DIR}/tests ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/src/common ${PROJECT_SOURCE_DIR}/src/layer)
aux_source_directory(. srcs)

add_executable(test ${srcs})
target_link_libraries(test welkin)