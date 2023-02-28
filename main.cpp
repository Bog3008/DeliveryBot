
#include "opencv2/opencv.hpp"
#include <iostream>



int main()
{
    //// 1. Create mask control window with default HSV range to detect blue color
    auto const MASK_WINDOW = "Mask Settings";
    cv::namedWindow(MASK_WINDOW, cv::WINDOW_AUTOSIZE);

    // HSV range to detect blue color
    int minHue = 320/2, maxHue = 350/2;
    int minSat = 74, maxSat = 255;
    int minVal =  0, maxVal = 255;

    // Create trackbars in mask settings window
    cv::createTrackbar("Min Hue", MASK_WINDOW, &minHue, 360);
    cv::createTrackbar("Max Hue", MASK_WINDOW, &maxHue, 360);
    cv::createTrackbar("Min Sat", MASK_WINDOW, &minSat, 255);
    cv::createTrackbar("Max Sat", MASK_WINDOW, &maxSat, 255);
    cv::createTrackbar("Min Val", MASK_WINDOW, &minVal, 255);
    cv::createTrackbar("Max Val", MASK_WINDOW, &maxVal, 255);

    //// 2. Capture from default camera
    cv::VideoCapture videoCapture("C:\\Users\\floma\\CLionProjects\\DeliveryBot\\video_test_example\\test1.mp4");
    // C:\Users\floma\CLionProjects\DeliveryBot\video_test_example\test1.mp4
    if(!videoCapture.isOpened()){
        std::cout << "Error opening video stream or file" << std::endl;
        return -1;
    }


    while (true) {
        //// 3. Capture and convert video to HSV color space
        cv::Mat inputVideo;
        videoCapture.read(inputVideo);
        cv::flip(inputVideo, inputVideo, 1);
        cv::Mat inputVideoHSV;
        cv::cvtColor(inputVideo, inputVideoHSV, cv::COLOR_BGR2HSV);

        //// 4. Create mask and result (masked) video
        cv::Mat mask;
        // params: input array, lower boundary array, upper boundary array, output array
        cv::inRange(
                inputVideoHSV,
                cv::Scalar(minHue, minSat, minVal),
                cv::Scalar(maxHue, maxSat, maxVal),
                mask
        );
        cv::Mat resultVideo;
        // params: src1	array, src2 array, output array, mask
        cv::bitwise_and(inputVideo, inputVideo, resultVideo, mask);

        //// 5. Show videos
        //RESIZE NOCOMMIT
        resize(inputVideo, inputVideo, cv::Size_(inputVideo.cols/2, inputVideo.rows/2));
        resize(resultVideo, resultVideo, cv::Size_(resultVideo.cols/2, resultVideo.rows/2));// to half size or even smaller
        //namedWindow( "Display frame",cv::WINDOW_AUTOSIZE);
        //^^^
        cv::imshow("Mask", mask);
        cv::imshow("Input Video", inputVideo);
        cv::imshow("Result (Masked) Video", resultVideo);
        // cv::imshow("Mask", mask);

        //// Wait for 'esc' (27) key press for 30ms. If pressed, end program.
        if (cv::waitKey(30) == 27) break;
    }
}