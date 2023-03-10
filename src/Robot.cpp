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
#include "OrderQueue.hpp"

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
    std::cout << "run"<<std::endl;
    while(true){
        if(!OrderQueue::empty()){
            do_clean();
            OrderQueue::pop();
        }
    }
}

void RobotTest::do_clean(){
    std::cout << "do_clean"<<std::endl;
    while(true) {
        cv::Mat frame = get_frame();
        ObjectsDetector obj_d(frame);
        double angle  = obj_d.get_angle_in_degrees();
        double dist = obj_d.get_distance_in_cm();

        if (need_to_turn(angle)) {
            turn(angle);
            continue;
        }
        if (need_to_move(dist)) {
            move_forward(dist);
            continue;
        }
        break;
    }
}

bool RobotTest::need_to_turn(double current_angle){
    std::cout << "need_to_turn"<<std::endl;
    if(current_angle > 20)
        return true;
    return false;
}
bool RobotTest::need_to_move(double current_distance){
    std::cout << "need_to_move"<<std::endl;
    if(current_distance > 30)
        return true;
    return false;
}

cv::Mat RobotTest::get_frame(){
    std::cout << "get frame"<<std::endl;
    cv::Mat frame;
    cap >> frame;
    if(frame.empty()){
        throw std::runtime_error("Robot got empty frame, may be video stream is closed of end of file");
    }
    return frame;
}

void RobotTest::turn(double angle_before){
    turn_right(angle_before);

    cv::Mat frame = get_frame();
    int angle_after = ObjectsDetector(frame).get_angle_in_degrees();

    if(angle_after > angle_before){
        turn_left(angle_after);
    }
}
void RobotTest::move_ahead(){
    std::cout << "move_ahead"<<std::endl;
    cv::Mat frame = get_frame();
    int distance = ObjectsDetector(frame).get_distance_in_cm();
    move_forward(distance);
}