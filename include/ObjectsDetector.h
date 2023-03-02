
#pragma once

#include "opencv2/opencv.hpp"
#include "Point.h"
#include "HSVColor.h"
struct IObjectsDetector{
    virtual double get_angle_in_degrees() const = 0;
    virtual double get_destination_point() const = 0;
    virtual ~IObjectsDetector(){}
};

struct ObjectsDetector: public IObjectsDetector{
private:
    double angle_in_dergees, distance_in_meters;
public:
    ObjectsDetector() = delete;
    ObjectsDetector(cv::Mat&);

    double get_angle_in_degrees() const override;
    double get_destination_point() const override;

    double angle_between_three_points(cv::Point, cv::Point, cv::Point);
    cv::Point get_vec_by_two_points(cv::Point, cv::Point);
    double modulus_of_vec(cv::Point);

    void fill_mask_of_HSV_color(cv::Mat&, cv::Mat&, HSVColor&, HSVColor&);
    cv:: Point get_centoid(cv::Mat&);

    ~ObjectsDetector() = default;

};
void test_centroid(cv::Mat src);
void test2_centroid(cv::Mat src);