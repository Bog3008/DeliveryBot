
#pragma once
#ifndef DELIVERYBOT_VIDEOCAPTURER_H
#define DELIVERYBOT_VIDEOCAPTURER_H

#include "OpenCVHeaders.h"

#endif //DELIVERYBOT_VIDEOCAPTURER_H

struct IVideoCapturer{
    virtual void create_stream(std::string path);
    virtual cv::Mat && get_frame();
    virtual ~IVideoCapturer() = 0;
};

struct VideoCapturerTest: public IVideoCapturer{
private:
    cv::Mat img;
    void create_stream(std::string path) override;
    cv::Mat && get_frame() override;
    ~VideoCapturerTest(){}
};