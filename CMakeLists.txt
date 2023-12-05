cmake_minimum_required(VERSION 3.10)

project(GravitySim VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(GravitySim ${CMAKE_SOURCE_DIR}/src/main.cpp)

find_package(OpenGL REQUIRED)

file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/dependencies/include)
add_library(GLAD ${CMAKE_SOURCE_DIR}/src/glad.c)

target_link_libraries(GravitySim GLAD)

add_executable(GravitySimExe ${CMAKE_SOURCE_DIR}/src/main.cpp ${SOURCES})

target_link_libraries(GravitySim OpenGL::GL)
target_link_libraries(GravitySim ${CMAKE_SOURCE_DIR}/dependencies/libs/glfw3.lib)


target_link_libraries(GravitySimExe GravitySim)