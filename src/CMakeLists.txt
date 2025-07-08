cmake_minimum_required(VERSION 3.16)

find_package(glfw3 REQUIRED)

file(GLOB_RECURSE SOURCES "*.cpp")

list(FILTER SOURCES EXCLUDE REGEX "platform/.*$")

if ((CMAKE_SYSTEM_NAME STREQUAL "Linux") OR (CMAKE_SYSTEM_NAME STREQUAL "Windows") OR (CMAKE_SYSTEM_NAME STREQUAL "Darwin"))
    file(GLOB_RECURSE DESKTOP_SOURCES "platform/desktop/*.cpp")
    list(APPEND SOURCES ${DESKTOP_SOURCES})
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(Game ${SOURCES})
target_link_libraries(Game PRIVATE lua_static glad glfw)