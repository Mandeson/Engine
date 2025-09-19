cmake_minimum_required(VERSION 3.16)

set(SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/glad.c")

set_source_files_properties(${SOURCES} PROPERTIES LANGUAGE CXX)
add_library(glad ${SOURCES})
