
cmake_minimum_required(VERSION 3.23)
project(DeliveryBot)

set(CMAKE_CXX_STANDARD 23)
set(ENV{OPENCV_DIR} "E:\\Programs\\OpenCV\\opencv\\build")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(DeliveryBot main.cpp)

set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)

target_link_libraries(DeliveryBot ${OpenCV_LIBS})