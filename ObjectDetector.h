
#pragma once
#ifndef DELIVERYBOT_OBJECTDETECTOR_H
#define DELIVERYBOT_OBJECTDETECTOR_H

#endif //DELIVERYBOT_VIDEOCAPTURER_H

#include "opencv2/opencv.hpp"
#include "Point.h"

struct IObjectDetector{
    virtual ~IObjectDetector() = 0;
};

struct ObjectDetector: public IObjectDetector{
private:
    Point robot{0,0}, destination{0,0};
public:
    ObjectDetector() = delete;
    ObjectDetector(const cv::Mat&);
    Point get_robot_point();
    Point get_destination_point();
};