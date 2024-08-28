cmake_minimum_required(VERSION 3.25)
project(cv)

set(OpenCV_DIR C:/opencv/opencv/build)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
set(CMAKE_CXX_STANDARD 17)

add_executable(
        cv
        main.cpp
        src/ImageProcess.cpp
        include/ImageProcess.h
)
target_link_libraries(cv ${OpenCV_LIBS})
