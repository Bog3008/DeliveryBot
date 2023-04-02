#include "opencv2/opencv.hpp"
#include <iostream>
#include "ObjectsDetector.hpp"
#include <cmath>
#include "Robot.hpp"
#include "OrderQueue.hpp"
#include <stdio.h>
#include <Publisher.h>
#include "TgBot.h"

//cd E:\Clion Projects\DelBotSub\mosquitto\build\client\Debug

int main() {

    std::string topic = "test/t1";
    std::string host = "localhost";
    int port = 1883;

    tg_bot_run();

    MPublisher Publisher(host, port);
    Publisher.publish(topic, "Hell");
    for(int i = 0; i < 5; i++)
        Publisher.publish(topic, std::to_string(i));

    /*
    OrderQueue::add(10);
    RobotTest delbot("E:\\Clion Projects\\DeliveryBot\\video_test_example\\destin.mp4");
    try {
        delbot.run();
    }
    catch(std::runtime_error e){
        std::cout << e.what();
    }
     */
}