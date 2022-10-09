cmake_minimum_required(VERSION 3.15)

project(xtransport C CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(
    GLOB_RECURSE XTRANSPORT_SRC
    ${CMAKE_SOURCE_DIR}/src/*.h
    ${CMAKE_SOURCE_DIR}/src/*.c
)

include_directories(third_party/CFAST/include/ src/)
add_library(xtransport ${XTRANSPORT_SRC})

add_subdirectory(third_party/CFAST)
if (UNIX)
    target_link_libraries(xtransport pthread cfast)
else()
    target_link_libraries(xtransport cfast)
endif()


add_executable(
    xt_server
    ${CMAKE_SOURCE_DIR}/samples/server.c
)
target_link_libraries(xt_server xtransport)

add_executable(
    xt_client
    ${CMAKE_SOURCE_DIR}/samples/client.c
)
target_link_libraries(xt_client xtransport)
