cmake_minimum_required(VERSION 3.25)
project(lab2)

set(CMAKE_CXX_STANDARD 17)
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

link_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/lib
)
add_executable(lab2 main.cpp)

target_link_libraries(
        lab2
        opencv_world490d
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resource DESTINATION ${CMAKE_CURRENT_BINARY_DIR})