
#include "opencv2/opencv.hpp"
#include <iostream>
#include "ObjectsDetector.h"
#include "Point.h"
#include "HSVColor.h"
#include <math.h>
int main() {
    //// 1. Create mask control window with default HSV range to detect blue color

    // HSV range to detect blue color
    int minHue = 320 / 2, maxHue = 350 / 2;
    int minSat = 74, maxSat = 255;
    int minVal = 0, maxVal = 255;


    //// 2. Capture from default camera
    cv::VideoCapture cap("E:\\Clion Projects\\DeliveryBot\\video_test_example\\destin.mp4");
    // E:\Clion Projects\DeliveryBot\video_test_example\test1.mp4
    // C:\Users\floma\CLionProjects\DeliveryBot\video_test_example\test1.mp4
    // E:\Clion Projects\DeliveryBot\video_test_example\test1ShortVer.mp4
    //E:\Clion Projects\DeliveryBot\video_test_example\testPic.jpg
    if (!cap.isOpened()) {
        std::cout << "Error opening video stream or file" << std::endl;
        return -1;
    }


    while (1) {
        int frame_skiper = 0;
        //// 3. Capture and convert video to HSV color space
        cv::Mat inputVideoFrame;
        //cap.read(inputVideoFrame);
        cap >> inputVideoFrame;
        if (inputVideoFrame.empty()) {
            std::cout << "end of file\n";
            break;
        }
        if(frame_skiper % 1200 == 0){
            cv::flip(inputVideoFrame, inputVideoFrame, 1);
            ObjectsDetector OD{inputVideoFrame};
            frame_skiper = 0;
        }
        frame_skiper++;

        //// Wait for 'esc' (27) key press for 30ms. If pressed, end program.
        if (cv::waitKey(30) == 27)
            break;
    }
    cap.release();

    // Closes all the frames
    cv::destroyAllWindows();
}