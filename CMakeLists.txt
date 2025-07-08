cmake_minimum_required(VERSION 3.16)

project(Game LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_subdirectory(lib/Lua)
add_subdirectory(lib/Glad)
add_subdirectory(src)
