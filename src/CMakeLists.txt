cmake_minimum_required(VERSION 3.16)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    find_package(Lua REQUIRED)
    find_package(glm REQUIRED)
    find_package(Freetype REQUIRED)
    if (DESKTOP)
        find_package(glfw3 REQUIRED)
    endif()
else()
    include(FetchContent)

    FetchContent_Declare(
        Lua
        GIT_REPOSITORY	https://github.com/walterschell/Lua.git
        GIT_TAG 	3ed55a56eaa05c9221f40b3c07d0e908eb1067b0
    )
    FetchContent_MakeAvailable(Lua)

    FetchContent_Declare(
        glm
        GIT_REPOSITORY	https://github.com/g-truc/glm.git
        GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
    )
    FetchContent_MakeAvailable(glm)

    FetchContent_Declare(
        freetype
        GIT_REPOSITORY	https://github.com/freetype/freetype.git
        GIT_TAG 	b04db3872cfac5ee4fd4f895933cebce3576fbf0
    )
    FetchContent_MakeAvailable(freetype)

    if (DESKTOP)
        FetchContent_Declare(
            glfw
            GIT_REPOSITORY	https://github.com/glfw/glfw.git
            GIT_TAG 	ac10768495837eb98da27d01fe706073d6d251c2
        )
        FetchContent_MakeAvailable(glfw)
    endif()
endif()

file(GLOB_RECURSE SOURCES "*.cpp")

list(FILTER SOURCES EXCLUDE REGEX "platform/.*$")

if (DESKTOP)
    file(GLOB_RECURSE DESKTOP_SOURCES "platform/desktop/*.cpp")
    list(APPEND SOURCES ${DESKTOP_SOURCES})
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(Game ${SOURCES})
target_link_libraries(Game PRIVATE glad glm::glm)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_libraries(Game PRIVATE lua Freetype::Freetype)
else()
    target_link_libraries(Game PRIVATE lua_static freetype)
endif()

if (DESKTOP)
    target_link_libraries(Game PRIVATE glfw)
endif()
