//
// Created by Bogdan on 27.02.2023.
//
#pragma once
#include "ObjectsDetector.h"
#include "opencv2/opencv.hpp"
#include "Point.h"
#include "HSVColor.h"
Point ObjectsDetector::get_angle_between() const {
    return robot;
}
Point ObjectsDetector::get_destination_point() const {
    return destination;
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


    //test_centroid(mask_red);  finding ctnet but in simple shapes
    //test2_centroid(mask_red); finding blobs

    cv::Point red_center = get_centoid(mask_red);
    cv::Point blue_center = get_centoid(mask_blue);
    cv::Point destination_center = get_centoid(mask_des);
    cv::Point middle_of_robot_line = { (red_center.x + blue_center.x)/2,
                                       (red_center.y + blue_center.y)/2};
    /*
    std::cout << "\n\nAAA\n\n" <<"";
    std::cout << mask_red.rows <<" "<< mask_red.cols <<"\n" << red_center.x <<" " << red_center.y <<"\n";
    std::cout << mask_blue.rows <<" "<< mask_blue.cols <<"\n" << blue_center.x <<" " << blue_center.y <<"\n";
    */
    if(red_center.x < 0 && red_center.y < 0 &&
       blue_center.x < 0 && blue_center.y < 0 &&
       destination_center.x < 0 && destination_center.y < 0){
        std::cout <<"\n ROBOT IS OUT OF FIELD OF VIEW\n";

        return;
    }

    //double distance_between = abs(cv::norm(red_center-blue_center));

    cv::line(inputVideoFrame, destination_center, middle_of_robot_line, cv::Scalar(100, 200, 255), 5, cv::LINE_4);
    cv::line(inputVideoFrame, red_center, blue_center, cv::Scalar(0,250,200), 5, cv::LINE_4);

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

void test_centroid(cv::Mat src){
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
void test2_centroid(cv::Mat im) {
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