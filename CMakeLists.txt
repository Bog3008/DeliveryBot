
cmake_minimum_required(VERSION 3.23)
project(DeliveryBot)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(Boost_USE_MULTITHREADED ON)

set(BOOST_ROOT "E:/boost_1_62_0/boost_1_62_0/boost" CACHE PATH "Path to Boost directory")
set(Boost_INCLUDE_DIR "${BOOST_ROOT}/include" CACHE PATH "Path to Boost include directory")
set(Boost_LIBRARY_DIR "${BOOST_ROOT}/lib" CACHE PATH "Path to Boost library directory")

if(IS_DIRECTORY("E:\\Programs\\OpenCV\\opencv\\build"))
    set(ENV{OPENCV_DIR} "E:\\Programs\\OpenCV\\opencv\\build")
elseif(IS_DIRECTORY("D:\\tools\\opencv\\build"))
    set(ENV{OPENCV_DIR} "D:\\tools\\opencv\\build")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/mosquitto)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/mosquitto/include)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/mosquitto/build)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/mosquitto/build/lib/Debug)

#include_directories(E:/boost_1_62_0/boost_1_62_0/boost)

#set(Boost_USE_MULTITHREADED ON)
SET(BOOST_INCLUDE_DIR "C:/Users/Bogdan/.clion-vcpkg/vcpkg/packages/boost-system_x86-windows/include")

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)
find_package(CURL)
include_directories( ${OPENSSL_INCLUDE_DIR} ${Boost_INCLUDE_DIR})
if (CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIRS})
    add_definitions(-DHAVE_CURL)
endif()


add_executable(DeliveryBotBinary
        main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ObjectsDetector.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/OrderQueue.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Robot.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Publisher.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/TgBot.cpp
        )

target_include_directories(DeliveryBotBinary PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(DeliveryBotBinary
        ${OpenCV_LIBS}
        ${CMAKE_CURRENT_SOURCE_DIR}/mosquitto/build/lib/Debug/mosquitto.lib
        C:/Users/Bogdan/.clion-vcpkg/vcpkg/packages/tgbot-cpp_x64-windows/debug/lib/TgBot.lib
        ${CMAKE_THREAD_LIBS_INIT}
        ${OPENSSL_LIBRARIES}
        ${Boost_LIBRARIES}
        ${CURL_LIBRARIES}
        )
