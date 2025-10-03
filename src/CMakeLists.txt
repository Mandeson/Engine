cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(ENGINE_USE_SYSTEM_LUA_LIB_DEFAULT ON)
    set(ENGINE_USE_SYSTEM_GLM_LIB_DEFAULT ON)
    set(ENGINE_USE_SYSTEM_FREETYPE_LIB_DEFAULT ON)
    set(ENGINE_USE_SYSTEM_NLOHMANN_JSON_LIB_DEFAULT ON)
else()
    set(ENGINE_USE_SYSTEM_LUA_LIB_DEFAULT OFF)
    set(ENGINE_USE_SYSTEM_GLM_LIB_DEFAULT OFF)
    set(ENGINE_USE_SYSTEM_FREETYPE_LIB_DEFAULT OFF)
    set(ENGINE_USE_SYSTEM_NLOHMANN_JSON_LIB_DEFAULT OFF)
endif()

option(ENGINE_USE_SYSTEM_LUA_LIB "Use system Lua library" ${ENGINE_USE_SYSTEM_LUA_LIB_DEFAULT})
option(ENGINE_USE_SYSTEM_GLM_LIB "Use system glm library" ${ENGINE_USE_SYSTEM_GLM_LIB_DEFAULT})
option(ENGINE_USE_SYSTEM_FREETYPE_LIB "Use system Freetype library" ${ENGINE_USE_SYSTEM_FREETYPE_LIB_DEFAULT})
option(ENGINE_USE_SYSTEM_NLOHMANN_JSON_LIB "Use system nlohmann_json library" ${ENGINE_USE_SYSTEM_NLOHMANN_JSON_LIB_DEFAULT})

include(FetchContent)

if (ENGINE_USE_SYSTEM_LUA_LIB)
    find_package(Lua REQUIRED)
else()
    FetchContent_Declare(
        Lua
        GIT_REPOSITORY	https://github.com/walterschell/Lua.git
        GIT_TAG 	3ed55a56eaa05c9221f40b3c07d0e908eb1067b0
    )
    FetchContent_MakeAvailable(Lua)
endif()

if (ENGINE_USE_SYSTEM_GLM_LIB)
    find_package(glm REQUIRED)
else()
    FetchContent_Declare(
        glm
        GIT_REPOSITORY	https://github.com/g-truc/glm.git
        GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
    )
    FetchContent_MakeAvailable(glm)
endif()

if (ENGINE_USE_SYSTEM_FREETYPE_LIB)
    find_package(Freetype REQUIRED)
else()
    FetchContent_Declare(
        freetype
        GIT_REPOSITORY	https://github.com/freetype/freetype.git
        GIT_TAG 	b04db3872cfac5ee4fd4f895933cebce3576fbf0
    )
    FetchContent_MakeAvailable(freetype)
endif()

if (ENGINE_USE_SYSTEM_NLOHMANN_JSON_LIB)
    find_package(nlohmann_json REQUIRED)
else()
    if(POLICY CMP0135)
        cmake_policy(SET CMP0135 NEW)
        set(CMAKE_POLICY_DEFAULT_CMP0135 NEW)
    endif()
    FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
    FetchContent_MakeAvailable(json)
endif()

file(GLOB_RECURSE SOURCES "*.cpp")

list(FILTER SOURCES EXCLUDE REGEX "platform/.*$")

set(LIBRARIES upng glm::glm nlohmann_json::nlohmann_json)

if (ENGINE_DESKTOP)
    if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set(ENGINE_USE_SYSTEM_GLFW_LIB_DEFAULT ON)
    else()
        set(ENGINE_USE_SYSTEM_GLFW_LIB_DEFAULT OFF)
    endif()
    option(ENGINE_USE_SYSTEM_GLFW_LIB "Use system GLFW library" ${ENGINE_USE_SYSTEM_GLFW_LIB_DEFAULT})
    if (ENGINE_USE_SYSTEM_GLFW_LIB)
        find_package(glfw3 REQUIRED)
    else()
        FetchContent_Declare(
            glfw
            GIT_REPOSITORY	https://github.com/glfw/glfw.git
            GIT_TAG 	ac10768495837eb98da27d01fe706073d6d251c2
        )
        FetchContent_MakeAvailable(glfw)
    endif()
    file(GLOB_RECURSE ENGINE_DESKTOP_SOURCES "platform/desktop/*.cpp")
    list(APPEND SOURCES ${ENGINE_DESKTOP_SOURCES})
    if (ENGINE_DESKTOP_EXECUTABLE_NAME)
        set(TARGET ${ENGINE_DESKTOP_EXECUTABLE_NAME})
    else()
        set(TARGET Game)
    endif()
    add_executable(${TARGET} ${SOURCES})
    list(APPEND LIBRARIES glad glfw)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Android")
    find_package(game-activity REQUIRED CONFIG)
    set(CMAKE_SHARED_LINKER_FLAGS
            "${CMAKE_SHARED_LINKER_FLAGS} -u \
            Java_com_google_androidgamesdk_GameActivity_initializeNativeCode")
    file(GLOB_RECURSE ANDROID_SOURCES "platform/android/*.cpp")
    list(APPEND SOURCES ${ANDROID_SOURCES})
    set(TARGET engine)
    add_library(${TARGET} SHARED ${SOURCES})
endif()

target_compile_definitions(${TARGET} PUBLIC "$<$<CONFIG:DEBUG>:ENGINE_DEBUG>")
if (ENGINE_MEMORY_SANITIZER)
    target_compile_options(${TARGET} PUBLIC -fsanitize=hwaddress -fno-omit-frame-pointer)
    target_link_options(${TARGET} PUBLIC -fsanitize=hwaddress)
endif()

if (ENGINE_USE_SYSTEM_LUA_LIB)
    list(APPEND LIBRARIES lua)
else()
    list(APPEND LIBRARIES lua_static)
endif()
if (ENGINE_USE_SYSTEM_FREETYPE_LIB)
    list(APPEND LIBRARIES Freetype::Freetype)
else()
    list(APPEND LIBRARIES freetype)
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

if (ENGINE_DESKTOP)
    target_link_libraries(${TARGET} PRIVATE ${LIBRARIES})
    if (ENGINE_DESKTOP_RUNTIME_DIR)
        set_target_properties(${TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${ENGINE_DESKTOP_RUNTIME_DIR})
    else()
        set_target_properties(${TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})
    endif()
else()
    target_link_libraries(${TARGET} ${LIBRARIES})
endif()

if (ENGINE_ASSETS_DIR_COPY)
    add_custom_command(TARGET ${TARGET} POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/assets" "${ENGINE_ASSETS_DIR_COPY_PATH}/assets")
endif()
