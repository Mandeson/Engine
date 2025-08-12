cmake_minimum_required(VERSION 3.16)

set(SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/upng/upng.c")

set_source_files_properties(${SOURCES} PROPERTIES LANGUAGE CXX)
add_library(upng ${SOURCES})
target_include_directories(upng PUBLIC "${CMAKE_SOURCE_DIR}/lib/upng/upng/include/")
