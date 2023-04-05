
#pragma once

#include "opencv2/opencv.hpp"
//#include "Point.h"
#include "HSVColor.h"
struct IObjectsDetector{
    virtual double get_angle_in_degrees() const = 0;
    virtual double get_distance_in_cm() const = 0;
    virtual ~IObjectsDetector(){}
};

struct ObjectsDetector: public IObjectsDetector{
private:
    double angle_in_dergees, distance_in_cm;
public:
    ObjectsDetector() = delete;
    explicit ObjectsDetector(cv::Mat&);

    double get_angle_in_degrees() const noexcept override;
    double get_distance_in_cm() const noexcept override;

    static double angle_between_three_points(cv::Point, cv::Point, cv::Point);
    static cv::Point get_vec_by_two_points(cv::Point, cv::Point);
    static double modulus_of_vec(cv::Point);
    static double distance_between_two_points(cv::Point, cv::Point);

    static void fill_mask_of_HSV_color(cv::Mat&, cv::Mat&, HSVColor&, HSVColor&);
    static cv:: Point get_centoid(cv::Mat&);

    ~ObjectsDetector() = default;


};
cv::Point color_center(cv::Mat);
void test_centroid(cv::Mat src);
void test2_centroid(cv::Mat src);