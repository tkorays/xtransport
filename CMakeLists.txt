cmake_minimum_required(VERSION 3.15)

project(xtransport C CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(
    GLOB_RECURSE XTRANSPORT_SRC
    ${CMAKE_SOURCE_DIR}/src/*.h
    ${CMAKE_SOURCE_DIR}/src/*.c
)

add_library(xtransport ${XTRANSPORT_SRC})

