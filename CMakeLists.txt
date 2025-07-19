cmake_minimum_required(VERSION 3.16)

project(Game LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-g3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

if ((CMAKE_SYSTEM_NAME STREQUAL "Linux") OR (CMAKE_SYSTEM_NAME STREQUAL "Windows") OR (CMAKE_SYSTEM_NAME STREQUAL "Darwin"))
    set(DESKTOP TRUE)
    add_subdirectory(lib/Glad)
endif()

add_subdirectory(src)
