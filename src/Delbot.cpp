

#include "Delbot.hpp"
#include "Publisher.h"
#include "opencv2/opencv.hpp"
#include "Data.h"
#include "ObjectsDetector.h"
#include <atomic>
#include <future>
#include <mutex>
#include <condition_variable>

std::atomic<double> atomic_dist{-1};
std::atomic<double> atomic_angle{-1};
std::atomic<bool> atomic_stop_sending{0};

void Delbot::run(){
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cout << "Error opening video stream or file" << std::endl;
        throw std::runtime_error("Couldnt open video stream");
    }

    std::future<void> result = std::async(std::launch::async, async_sender);

    while (1) {
        int frame_skiper = 0;
        cv::Mat inputVideoFrame;

        cap >> inputVideoFrame;
        if (inputVideoFrame.empty()) {
            std::cout << "end of file\n";
            break;
        }

        if(frame_skiper % 1200 == 0){
            cv::flip(inputVideoFrame, inputVideoFrame, 1);
            ObjectsDetector OD{inputVideoFrame};

            double angle = OD.get_angle_in_degrees();
            double dist = OD.get_distance_in_cm();
            //std::cout<< "Angle: " << angle
            //         << "\nDist: " << dist <<std::endl;
            if(dist < 1000) {
                atomic_angle = angle;
                atomic_dist = dist;
            }
            if(angle > 15){
                //std::future<void>
                //std::async(std::launch::async, turn_left, 15);
                //turn_left(15);
            }
            else{
                if(dist > 20) {
                    //std::async(std::launch::async, turn_left, 15);
                    //move_forward(10);
                }
            }
            frame_skiper = 0;
        }
        frame_skiper++;


        if (cv::waitKey(30) == 27) {
            atomic_stop_sending = true;
            break;
        }
    }
    cap.release();
    atomic_stop_sending = true;
    cv::destroyAllWindows();
}

void Delbot::turn_left(int degrees) {
    std::cout <<"turn left on "<< degrees <<std::endl;
    std::string message = "{\"cmd\":\"left\",\"val\":1.0}";
    std::cout << "mes is: " << message << std::endl;
    MPublisher p(Data::host, Data::port);
    p.publish(Data::topic, message);
}
void Delbot::turn_right(int degrees){
    std::cout <<"turn right on "<< degrees <<std::endl;
    std::string message = "{\"cmd\":\"right\",\"val\":1.0}";
    std::cout << "mes is: " << message << std::endl;

    MPublisher p(Data::host, Data::port);
    p.publish(Data::topic, message);
}
void Delbot::move_forward(int meters){
    std::cout <<"move forward on "<< meters <<std::endl;
    std::string message = "{\"cmd\":\"forward\",\"val\":1.0}";
    std::cout << "mes is: " << message << std::endl;
    MPublisher p(Data::host, Data::port);
    p.publish(Data::topic, message);
}

void async_sender(){
    int counter = 0;
    while(!atomic_stop_sending) {
        counter++;
        if (counter > 10000) {
            counter = 0;
            //std::cout << "a sender" << atomic_dist << std::endl;
            if (atomic_angle > 15) {
                //std::cout << "TURN ON " << atomic_angle << std::endl;
                //std::future<void>
                //std::async(std::launch::async, turn_left, 15);
                Delbot::turn_left(15);
            } else {
                if (atomic_dist > 20) {
                    //std::cout << "MOVE ON "<< atomic_dist << std::endl;
                    //std::async(std::launch::async, turn_left, 15);
                    Delbot::move_forward(10);
                }
            }
        }
    }
}