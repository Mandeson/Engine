cmake_minimum_required(VERSION 3.16)

if (CMAKE_SYSTEM_NAME STREQUAL "Android")
    set(SOURCES "gles2.c")
else()
    set(SOURCES "gl.c")
endif()

set_source_files_properties(${SOURCES} PROPERTIES LANGUAGE CXX)
add_library(glad ${SOURCES})
