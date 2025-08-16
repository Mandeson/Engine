cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    find_package(Lua REQUIRED)
    find_package(glm REQUIRED)
    find_package(Freetype REQUIRED)
    find_package(nlohmann_json REQUIRED)
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

    cmake_policy(SET CMP0135 NEW)
    FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
    FetchContent_MakeAvailable(json)
endif()

file(GLOB_RECURSE SOURCES "*.cpp")

list(FILTER SOURCES EXCLUDE REGEX "platform/.*$")

set(LIBRARIES upng glm::glm nlohmann_json::nlohmann_json)

if (DESKTOP)
    if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
        find_package(glfw3 REQUIRED)
    else()
        FetchContent_Declare(
            glfw
            GIT_REPOSITORY	https://github.com/glfw/glfw.git
            GIT_TAG 	ac10768495837eb98da27d01fe706073d6d251c2
        )
        FetchContent_MakeAvailable(glfw)
    endif()
    file(GLOB_RECURSE DESKTOP_SOURCES "platform/desktop/*.cpp")
    list(APPEND SOURCES ${DESKTOP_SOURCES})
    add_executable(Game ${SOURCES})
    list(APPEND LIBRARIES glad glfw)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Android")
    find_package(game-activity REQUIRED CONFIG)
    file(GLOB_RECURSE ANDROID_SOURCES "platform/android/*.cpp")
    list(APPEND SOURCES ${ANDROID_SOURCES})
    add_library(engine SHARED ${SOURCES})
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    list(APPEND LIBRARIES lua Freetype::Freetype)
else()
    list(APPEND LIBRARIES lua_static freetype)
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Android")
    list(APPEND LIBRARIES
        game-activity::game-activity
        EGL
        GLESv3
        jnigraphics
        android
        log)
endif()

if (DESKTOP)
    target_link_libraries(Game PRIVATE ${LIBRARIES})
else()
    target_link_libraries(engine ${LIBRARIES})
endif()
