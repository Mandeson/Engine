cmake_minimum_required(VERSION 3.16)

project(Engine LANGUAGES CXX)

find_package(Git QUIET)
execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                RESULT_VARIABLE GIT_SUBMODULE_RESULT)
if(NOT GIT_SUBMODULE_RESULT EQUAL "0")
    message(WARNING "Could not download git submodules. Check git install and internet connection.")
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Android")
    if ((CMAKE_ANDROID_ARCH STREQUAL "arm") OR (CMAKE_ANDROID_ARCH STREQUAL "x86"))
        # Old Android versions do not support fseeko and ftello on 32-bit systems.
        # However, these functions are used by Lua and I think that adding the macros below is better than
        # dropping support of older API versions or manually forking and patching Lua.
        add_compile_definitions(fseeko=fseek ftello=ftell)
    endif()
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if (NOT MSVC)
        add_compile_options(-g3)
    endif()
else()
    if (NOT MSVC)
        add_compile_options(-O3)
    endif()
endif()

if (MSVC)
    add_compile_options(/EHsc /O2)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
else()
    add_compile_options(-fexceptions)
endif()

if ((CMAKE_SYSTEM_NAME STREQUAL "Linux") OR (CMAKE_SYSTEM_NAME STREQUAL "Windows") OR (CMAKE_SYSTEM_NAME STREQUAL "Darwin"))
    set(ENGINE_DESKTOP TRUE)
    add_subdirectory(lib/Glad)
endif()
add_subdirectory(lib/upng)

if (NOT MSVC)
    set(CMAKE_CXX_FLAGS "-Wall")
endif()

add_subdirectory(src)
