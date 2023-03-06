//
// Created by Bogdan on 06.03.2023.
//
#pragma once
#include <iostream>
#include <queue>
#include "opencv2/opencv.hpp"
namespace {
    std::queue<int> order_queue;
}

struct IRobot{
    virtual void run();
    virtual void turn_left(int degrees);
    virtual void turn_right(int degrees);
    virtual void move_forward(int meters);
    virtual virtual ~IRobot() = 0;
};

struct Robot: public IRobot{
    virtual void run() override final;
    virtual void turn_left(int degrees) override final;
    virtual void turn_right(int degrees) override final;
    virtual void move_forward(int meters) override final;
};

struct RobotTest: public IRobot{
    cv::VideoCapture cap;

    RobotTest(const char * path): cap(path){
        if (!cap.isOpened()) {
            std::cout << "Error opening video stream or file" << std::endl;
            throw std::runtime_error("Robot videoCapture cant open the video stream");
        }
    }
    cv::Mat && get_frame();

    virtual void run() override final;
    virtual void turn_left(int degrees) override final;
    virtual void turn_right(int degrees) override final;
    virtual void move_forward(int meters) override final;

    void turn();
    void move();

    void do_clean();
    bool need_to_turn();
    bool need_to_move()
};