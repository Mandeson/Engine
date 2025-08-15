cmake_minimum_required(VERSION 3.16)

project(Game LANGUAGES CXX)

if (CMAKE_SYSTEM_NAME STREQUAL "Android")
    if ((CMAKE_ANDROID_ARCH STREQUAL "arm") OR (CMAKE_ANDROID_ARCH STREQUAL "x86"))
        # Old Android versions do not support fseeko and ftello on 32-bit systems.
        # However, these functions are used by Lua and I think that adding the macros below is better than
        # dropping support of older API versions or manually forking and patching Lua.
        add_compile_definitions(fseeko=fseek ftello=ftell)
    endif()
endif()

if (MSVC)
    set(CMAKE_CXX_FLAGS "/EHsc")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2")
else()
    set(CMAKE_CXX_FLAGS_DEBUG "-g3")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()

if ((CMAKE_SYSTEM_NAME STREQUAL "Linux") OR (CMAKE_SYSTEM_NAME STREQUAL "Windows") OR (CMAKE_SYSTEM_NAME STREQUAL "Darwin"))
    set(DESKTOP TRUE)
    add_subdirectory(lib/Glad)
endif()
add_subdirectory(lib/upng)

if (NOT MSVC)
    set(CMAKE_CXX_FLAGS "-Wall")
endif()
add_subdirectory(src)
