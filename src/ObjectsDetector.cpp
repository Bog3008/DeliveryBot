//
// Created by Bogdan on 27.02.2023.
//
#pragma once
#include "ObjectsDetector.h"
#include "opencv2/opencv.hpp"
#include "HSVColor.h"
#include <cmath>
using std::cout;
using std::endl;

cv::Point color_center(cv::Mat src){
    // Convert the image to grayscale
    //cv::Mat image = src;

    // Convert the image to grayscale
    cv::Mat thresh = src;
    std::vector<std::vector<cv::Point>> contours;

    cv::findContours(thresh, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

// Find the contour with the largest area
    double max_area = 0;
    int max_area_idx = -1;

    for (int i = 0; i < contours.size(); i++) {
        double area = cv::contourArea(contours[i]);
        if (area > max_area) {
            max_area = area;
            max_area_idx = i;
        }
    }

// Find the center of the contour
    if(max_area_idx == -1)
        return {-1,-1};
    cv::Moments moments = cv::moments(contours[max_area_idx]);
    cv::Point center(moments.m10 / moments.m00, moments.m01 / moments.m00);

// Draw a circle at the center of the brightest spot
    cv::circle(src, center, 5, cv::Scalar(0, 0, 255), -1);

// Display the result
    //cv::imshow("Result", image);
    //cv::waitKey(0);
    return center;
}

double ObjectsDetector::get_angle_in_degrees() const noexcept{
    return angle_in_dergees;
}
double ObjectsDetector::get_distance_in_cm() const noexcept{
    return distance_in_cm;
}


ObjectsDetector::ObjectsDetector( cv::Mat& inputVideoFrame) {
    if (inputVideoFrame.empty()) {
        throw std::logic_error("empty file recive in ObjectsDetector ctor");
    }

    HSVColor min_hsv_threshold_red{160, 74, 70};
    HSVColor max_hsv_threshold_red{175, 255, 255};

    HSVColor min_hsv_threshold_blue{100, 74, 70};
    HSVColor max_hsv_threshold_blue{145, 255, 255};

    HSVColor min_hsv_threshold_green{54, 100, 70};
    HSVColor max_hsv_threshold_green{94, 255, 255};

    cv::Mat mask_blue;
    cv::Mat mask_red;
    cv::Mat mask_des;

    fill_mask_of_HSV_color(inputVideoFrame, mask_red, min_hsv_threshold_red, max_hsv_threshold_red);
    fill_mask_of_HSV_color(inputVideoFrame, mask_blue, min_hsv_threshold_blue, max_hsv_threshold_blue);
    fill_mask_of_HSV_color(inputVideoFrame, mask_des, min_hsv_threshold_green, max_hsv_threshold_green);


    cv::Point red_center = color_center(mask_red);
    cv::Point blue_center = color_center(mask_blue);

    cv::Point& top_of_robot = red_center; // allias to comfortable chancge the direction of robot

    cv::Point destination_center = color_center(mask_des);
    cv::Point middle_of_robot_line = { (red_center.x + blue_center.x)/2,
                                       (red_center.y + blue_center.y)/2};
    cv::line(inputVideoFrame, destination_center, middle_of_robot_line, cv::Scalar(100, 200, 255), 5, cv::LINE_4);
    cv::line(inputVideoFrame, red_center, blue_center, cv::Scalar(0,250,200), 5, cv::LINE_4);
    cv::imshow("Input", inputVideoFrame);
    cv::imshow("Mask", mask_red + mask_blue + mask_des);

    if(red_center.x < 0 && red_center.y < 0 &&
       blue_center.x < 0 && blue_center.y < 0 &&
       destination_center.x < 0 && destination_center.y < 0){
        std::cout <<"\n ROBOT OR DISTIN IS OUT OF FIELD OF VIEW\n";
        angle_in_dergees = -1;
        distance_in_cm = -1;
        return;
    }


    angle_in_dergees = angle_between_three_points(top_of_robot, middle_of_robot_line, destination_center);

    double distance_between_robot_top_and_destination = distance_between_two_points(top_of_robot, destination_center);
    double size_of_robot = distance_between_two_points(top_of_robot, middle_of_robot_line) * 2;
    distance_in_cm = (distance_between_robot_top_and_destination / size_of_robot) * 20; // i supose the size of robot is 20cm

    //std::cout <<"angle in dergees is: " << angle_in_dergees << "\n";
    //cout << "distance: " << distance_in_cm <<endl;


    //cv::waitKey();

}
void ObjectsDetector::fill_mask_of_HSV_color(cv::Mat& inputVideoFrame, cv::Mat& mask, HSVColor& min_hsv, HSVColor& max_hsv){
    cv::Mat inputVideoFrameHSV;
    cv::cvtColor(inputVideoFrame, inputVideoFrameHSV, cv::COLOR_BGR2HSV);

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

}

cv::Point ObjectsDetector::get_centoid(cv::Mat& src){
    cv::Mat thr;

// convert grayscale to binary image
    threshold( src, thr, 100,255,cv::THRESH_BINARY );

// find moments of the image
    cv::Moments m = moments(thr,true);
    cv::Point p(m.m10/m.m00, m.m01/m.m00);

// show the image with a point mark at the centroid
    circle(src, p, 5, cv::Scalar(128,0,0), -1);
    //cv::imshow("Image with center",src);
    return p;
}

double ObjectsDetector::angle_between_three_points(cv::Point robot, cv::Point mid, cv::Point destin){
    cv::Point robot_vec = get_vec_by_two_points(mid, robot);
    cv::Point destin_vec = get_vec_by_two_points(mid, destin);

    //angle_in_dergees = (a * b) / (/a/*/b/)
    double numerator = (robot_vec.x * destin_vec.x) + (robot_vec.y * destin_vec.y);
    double denumerator = modulus_of_vec(robot_vec) * modulus_of_vec(destin_vec);

    double angle_in_di = acos(numerator/denumerator);
    return (angle_in_di*180)/3.14;
}
cv::Point ObjectsDetector::get_vec_by_two_points(cv::Point lhs, cv::Point rhs){
    // AB = (x2 - x1; y2-y1)
    return {rhs.x - lhs.x, rhs.y - lhs.y};
}
double ObjectsDetector::modulus_of_vec(cv::Point vec){
    return sqrt((vec.x * vec.x) + (vec.y * vec.y));
}

double ObjectsDetector::distance_between_two_points(cv::Point a, cv::Point b){
    return sqrt(pow((a.x-b.x), 2) + pow((a.y-b.y), 2));
}

