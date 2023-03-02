//
// Created by Bogdan on 27.02.2023.
//
#pragma once
#include "ObjectsDetector.h"
#include "opencv2/opencv.hpp"
#include "Point.h"
#include "HSVColor.h"
Point ObjectsDetector::get_robot_point() const {
    return robot;
}
Point ObjectsDetector::get_destination_point() const {
    return destination;
}

ObjectsDetector::ObjectsDetector( cv::Mat& inputVideoFrame) {
    if (inputVideoFrame.empty()) {
        throw std::logic_error("empty file recive in ObjectsDetector ctor");
    }

    // HSV range to detect RED(OR PURPLE idk) color
    /*
    int minHue = 320 / 2, maxHue = 350 / 2;
    int minSat = 74, maxSat = 255;
    int minVal = 0, maxVal = 255;
    */
    HSVColor min_hsv_threshold_red{160, 74, 0};
    HSVColor max_hsv_threshold_red{175, 255, 255};

    HSVColor min_hsv_threshold_blue{160, 74, 0};
    HSVColor max_hsv_threshold_blue{175, 255, 255};

    get_mask_of_HSV_color(inputVideoFrame, min_hsv_threshold_red, max_hsv_threshold_red);
    get_mask_of_HSV_color(inputVideoFrame, min_hsv_threshold_blue, max_hsv_threshold_blue);

}
cv::Mat&& ObjectsDetector::get_mask_of_HSV_color( cv::Mat& inputVideoFrame, HSVColor& min_hsv, HSVColor& max_hsv){
    cv::flip(inputVideoFrame, inputVideoFrame, 1);
    cv::Mat inputVideoFrameHSV;
    cv::cvtColor(inputVideoFrame, inputVideoFrameHSV, cv::COLOR_BGR2HSV);

    //// 4. Create mask and result (masked) video
    cv::Mat mask;
    // params: input array, lower boundary array, upper boundary array, output array
    cv::inRange(
            inputVideoFrameHSV,
            cv::Scalar(min_hsv.H, min_hsv.S, min_hsv.V),
            cv::Scalar(max_hsv.H, max_hsv.S, max_hsv.V),
            mask
    );
    cv::Mat resultVideoFrame;
    // params: src1	array, src2 array, output array, mask
    cv::bitwise_and(inputVideoFrame, inputVideoFrame, resultVideoFrame, mask);

    //// 5. Show videos
    resize(inputVideoFrame, inputVideoFrame, cv::Size_(inputVideoFrame.cols / 2, inputVideoFrame.rows / 2));
    resize(resultVideoFrame, resultVideoFrame,
           cv::Size_(resultVideoFrame.cols / 2, resultVideoFrame.rows / 2));// to half size or even smaller
    //namedWindow( "Display frame",cv::WINDOW_AUTOSIZE);

    cv::imshow("Mask", mask);
    cv::imshow("Input Video", inputVideoFrame);
    cv::imshow("Result (Masked) Video", resultVideoFrame);
    // cv::imshow("Mask", mask);
    return std::move(mask);
}