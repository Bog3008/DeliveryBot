//
// Created by Bogdan on 27.02.2023.
//

#include "ObjectDetector.h"

Point ObjectDetector::get_robot_point() {
    return robot;
}
Point ObjectDetector::get_destination_point() {
    return destination;
}
ObjectDetector::ObjectDetector(const cv::Mat&) {

}
