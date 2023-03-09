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

    HSVColor min_hsv_threshold_red{160, 74, 100};
    HSVColor max_hsv_threshold_red{175, 255, 255};

    HSVColor min_hsv_threshold_blue{80, 74, 100};
    HSVColor max_hsv_threshold_blue{145, 255, 255};

    HSVColor min_hsv_threshold_green{50, 100, 100};
    HSVColor max_hsv_threshold_green{70, 255, 255};

    cv::Mat mask_blue;
    cv::Mat mask_red;
    cv::Mat mask_des;

    fill_mask_of_HSV_color(inputVideoFrame, mask_red, min_hsv_threshold_red, max_hsv_threshold_red);
    fill_mask_of_HSV_color(inputVideoFrame, mask_blue, min_hsv_threshold_blue, max_hsv_threshold_blue);
    fill_mask_of_HSV_color(inputVideoFrame, mask_des, min_hsv_threshold_green, max_hsv_threshold_green);


    cv::Point red_center = BrSp(mask_red);
    //cv::Point red_center = get_centoid(mask_red);
    cv::Point blue_center = get_centoid(mask_blue);

    cv::Point& top_of_robot = red_center; // allias to comfortable chancge the direction of robot


    cv::Point destination_center = get_centoid(mask_des);
    cv::Point middle_of_robot_line = { (red_center.x + blue_center.x)/2,
                                       (red_center.y + blue_center.y)/2};

    if(red_center.x < 0 && red_center.y < 0 &&
       blue_center.x < 0 && blue_center.y < 0 &&
       destination_center.x < 0 && destination_center.y < 0){
        std::cout <<"\n ROBOT IS OUT OF FIELD OF VIEW\n";
        return;
    }


    cv::line(inputVideoFrame, destination_center, middle_of_robot_line, cv::Scalar(100, 200, 255), 5, cv::LINE_4);
    cv::line(inputVideoFrame, red_center, blue_center, cv::Scalar(0,250,200), 5, cv::LINE_4);

    angle_in_dergees = angle_between_three_points(top_of_robot, middle_of_robot_line, destination_center);

    double distance_between_robot_top_and_destination = distance_between_two_points(top_of_robot, destination_center);
    double size_of_robot = distance_between_two_points(top_of_robot, middle_of_robot_line) * 2;
    distance_in_cm = (distance_between_robot_top_and_destination / size_of_robot) * 20; // i supose the size of robot is 20cm

    std::cout <<"angle in dergees is: " << angle_in_dergees << "\n";
    cout << "distance: " << distance_in_cm <<endl;
    //cv::circle(inputVideoFrame, maxLoc, 100, cv::Scalar(0, 0, 255), 5);
    cv::imshow("Input", inputVideoFrame);
    cv::imshow("Mask", mask_red + mask_blue + mask_des);
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

cv::Point BrSp(cv::Mat src){
    // Convert the image to grayscale
    cv::Mat gray;
    return cv::Point{0,0};
    //cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);

    // Find the brightest spot in the image
    double maxVal;
    cv::Point maxLoc;
    cv::minMaxLoc(gray, NULL, &maxVal, NULL, &maxLoc);

    // Display the brightest spot on the original image
    //cv::circle(gray, maxLoc, 100, cv::Scalar(0, 0, 255), 5);
    //cv::imshow("Brightest Spot", gray);
    return maxLoc;
}
void test_centroid(cv::Mat& src){
    cv::Mat canny_output;
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;

// detect edges using canny
    cv::Canny( src, canny_output, 50, 150, 3 );

// find contours
    cv::findContours( canny_output, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

// get the moments
    std::vector<cv::Moments> mu(contours.size());
    for( int i = 0; i<contours.size(); i++ )
    { mu[i] = cv::moments( contours[i], false ); }

// get the centroid of figures.
    std::vector<cv::Point2f> mc(contours.size());
    for( int i = 0; i<contours.size(); i++)
    { mc[i] = cv::Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); }

// draw contours
    cv::Mat drawing(canny_output.size(), CV_8UC3, cv::Scalar(255,255,255));
    for( int i = 0; i<contours.size(); i++ )
    {
        cv::Scalar color = cv::Scalar(167,151,0); // B G R values
        cv::drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, cv::Point());
        cv::circle( drawing, mc[i], 4, color, -1, 8, 0 );
    }

// show the resultant image
    cv::namedWindow( "Contours", cv::WINDOW_AUTOSIZE );
    cv::imshow( "Contours", drawing );
    cv::waitKey(0);
}
void test2_centroid(cv::Mat& im) {
    // Setup SimpleBlobDetector parameters.
    cv::SimpleBlobDetector::Params params;

// Change thresholds
    params.minThreshold = 200;
    params.maxThreshold = 255;

// Filter by Area.
    params.filterByArea = false;
    //params.minArea = 2000;

// Filter by Circularity
    params.filterByCircularity = false;
    //params.minCircularity = 0.1;

// Filter by Convexity
    params.filterByConvexity = false;
    //params.minConvexity = 0.01;

// Filter by Inertia
    params.filterByInertia = false;
    //params.minInertiaRatio = 0.01;
    cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(params);

    //cv::Mat im = imread( "E:\\Clion Projects\\DeliveryBot\\video_test_example\\testPic.jpg", cv::IMREAD_GRAYSCALE );

// Set up the detector with default parameters.

// Detect blobs.
    std::vector<cv::KeyPoint> keypoints;
    detector->detect(im, keypoints);

// Draw detected blobs as red circles.
// DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures the size of the circle corresponds to the size of blob
    cv::Mat im_with_keypoints;
    cv::drawKeypoints(im, keypoints, im_with_keypoints, cv::Scalar(0, 0, 255),
                      cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

// Show blobs
    cv::imshow("keypoints", im_with_keypoints);
    cv::waitKey(0);
}