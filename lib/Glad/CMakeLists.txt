cmake_minimum_required(VERSION 3.16)

add_subdirectory(src)
target_include_directories(glad PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
