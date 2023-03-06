//
// Created by Bogdan on 06.03.2023.
//
#pragma once
#include <iostream>
#include <cmath>
#include "HSVColor.h"
#include "Point.h"
#include "opencv2/opencv.hpp"
#include "ObjectsDetector.hpp"
#include "Robot.hpp"

RobotTest::RobotTest(const char *path): cap(path){
    if (!cap.isOpened()) {
        std::cout << "Error opening video stream or file" << std::endl;
        throw std::runtime_error("Robot videoCapture cant open the video stream");
    }
}
RobotTest::RobotTest(int path): cap(path){
    if (!cap.isOpened()) {
        std::cout << "Error opening video stream or file" << std::endl;
        throw std::runtime_error("Robot videoCapture cant open the video stream");
    }
}
RobotTest::~RobotTest(){
    cap.release();
}

void RobotTest::turn_left(int degrees) {
        std::cout <<"turn left on "<< degrees <<std::endl;
}
void RobotTest::turn_right(int degrees){
        std::cout <<"turn right on "<< degrees <<std::endl;
}
void RobotTest::move_forward(int meters){
        std::cout <<"move forward on "<< meters <<std::endl;
}

void RobotTest::run() {
    while(true){
        if(true){ //add queue
            do_clean();
        }
    }
}

void RobotTest::do_clean(){
    while(true) {
        bool all_complete = 1;
        if (need_to_turn()) {
            turn();
            all_complete *= 0;
        }
        if (need_to_move() && all_complete) {
            move_ahead();
            all_complete *= 0;
        }
        if(all_complete)
            break;
    }
}

bool RobotTest::need_to_turn(){
    cv::Mat frame = get_frame();
    int current_angle = ObjectsDetector(frame).get_angle_in_degrees();
    if(current_angle > 20)
        return true;
    return false;
}
bool RobotTest::need_to_move(){
    cv::Mat frame = get_frame();
    int current_distance = ObjectsDetector(frame).get_distance_in_cm();
    if(current_distance > 30)
        return true;
    return false;
}

cv::Mat && RobotTest::get_frame(){
    cv::Mat frame;
    cap >> frame;
    if(frame.empty()){
        throw std::runtime_error("Robot got empty frame, may be video stream is closed of end of file");
    }
    return std::move(frame);
}

void RobotTest::turn(){
    cv::Mat frame = get_frame();
    int angle_before  = ObjectsDetector(frame).get_angle_in_degrees();
    turn_right(angle_before);

    frame = get_frame();
    int angle_after = ObjectsDetector(frame).get_angle_in_degrees();

    if(angle_after > angle_before){
        turn_left(angle_after);
    }
}
void RobotTest::move_ahead(){
    cv::Mat frame = get_frame();
    int distance = ObjectsDetector(frame).get_distance_in_cm();
    move_forward(distance);
}