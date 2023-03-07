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
#include <list>


struct IRobot{
    virtual void run() = 0;
    virtual void turn_left(int) = 0;
    virtual void turn_right(int ) = 0;
    virtual void move_forward(int ) = 0;
    virtual ~IRobot(){};
};


struct RobotTest: public IRobot{
    cv::VideoCapture cap;

    RobotTest(const char *);
    RobotTest(int);
    cv::Mat get_frame();

    void run() override final;
    void turn_left(int degrees) override final;
    void turn_right(int degrees) override final;
    void move_forward(int meters) override final;

    void turn();
    void move_ahead();

    void do_clean();
    bool need_to_turn();
    bool need_to_move();

    ~RobotTest();
};