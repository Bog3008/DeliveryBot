//
// Created by Bogdan on 27.02.2023.
//

#include "VideoCapturer.h"

void VideoCapturerTest::create_stream(std::string path){
    img = cv::imread(path, cv::IMREAD_COLOR);
}
cv::Mat && VideoCapturerTest::get_frame(){}
