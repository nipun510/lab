cmake_minimum_required(VERSION 3.10)

project(lab)
set(CMAKE_CXX_STANDARD 20)

list (APPEND INCLUDES "${PROJECT_SOURCE_DIR}")

add_subdirectory(json)
add_subdirectory(util)
add_subdirectory(test)




