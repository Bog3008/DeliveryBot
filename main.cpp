#include "opencv2/opencv.hpp"
#include <iostream>
#include "ObjectsDetector.hpp"
#include "Point.h"
#include "HSVColor.h"
#include <cmath>
#include "Robot.hpp"
#include "OrderQueue.hpp"

int main() {

    // cv::VideoCapture cap("E:\\Clion Projects\\DeliveryBot\\video_test_example\\destin.mp4");
    // E:\Clion Projects\DeliveryBot\video_test_example\test1.mp4
    // C:\Users\floma\CLionProjects\DeliveryBot\video_test_example\test1.mp4
    // E:\Clion Projects\DeliveryBot\video_test_example\test1ShortVer.mp4
    //E:\Clion Projects\DeliveryBot\video_test_example\testPic.jpg

    OrderQueue::add(10);

    RobotTest delbot("E:\\Clion Projects\\DeliveryBot\\video_test_example\\destin.mp4");
    try {
        delbot.run();
    }
    catch(std::runtime_error e){
        std::cout << e.what();
    }
}