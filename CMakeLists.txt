
cmake_minimum_required(VERSION 3.23)
project(DeliveryBot)
set(CMAKE_CXX_STANDARD 23)

if(IS_DIRECTORY("E:\\Programs\\OpenCV\\opencv\\build"))
    set(ENV{OPENCV_DIR} "E:\\Programs\\OpenCV\\opencv\\build")
elseif(IS_DIRECTORY("D:\\tools\\opencv\\build"))
    set(ENV{OPENCV_DIR} "D:\\tools\\opencv\\build")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/mosquitto/include)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/mosquitto)

add_executable(DeliveryBotBinary
        main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ObjectsDetector.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/OrderQueue.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Robot.cpp
        )

target_include_directories(DeliveryBotBinary PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(DeliveryBotBinary
        ${OpenCV_LIBS}
        ${CMAKE_CURRENT_SOURCE_DIR}/mosquitto/build/lib/Debug/mosquitto.lib
        )
