
cmake_minimum_required(VERSION 3.23)
project(DeliveryBot)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(Boost_USE_MULTITHREADED ON)



set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)

find_package (mosquitto REQUIRED COMPONENTS CXX_LIB)
include_directories (${mosquitto_INCLUDE})


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
        /vcpkg/packages/tgbot-cpp_x64-linux/debug/lib/libTgBot.a
        ${mosquitto}
        ${mosquitto_C_LIB}
        ${CMAKE_THREAD_LIBS_INIT}
        ${OPENSSL_LIBRARIES}
        ${Boost_LIBRARIES}
        ${CURL_LIBRARIES}
        )
