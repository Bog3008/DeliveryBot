
#pragma once

#include "opencv2/opencv.hpp"
#include "Point.h"
#include "HSVColor.h"

struct IObjectsDetector{
    virtual Point get_robot_point() const = 0;
    virtual Point get_destination_point() const = 0;
    virtual ~IObjectsDetector(){}
};

struct ObjectsDetector: public IObjectsDetector{
private:
    Point robot, destination;
public:
    ObjectsDetector() = delete;
    ObjectsDetector(cv::Mat&);

    Point get_robot_point() const override;
    Point get_destination_point() const override;
    cv::Mat&& get_mask_of_HSV_color( cv::Mat&, HSVColor&, HSVColor&) ;

    ~ObjectsDetector() = default;
};
