#include "opencv2/opencv.hpp"
#include <iostream>
#include "ObjectsDetector.hpp"
#include <cmath>
#include "Robot.hpp"
#include "OrderQueue.hpp"
#include <stdio.h>
#include <mosquitto.h>

int main() {

    // cv::VideoCapture cap("E:\\Clion Projects\\DeliveryBot\\video_test_example\\destin.mp4");
    // E:\Clion Projects\DeliveryBot\video_test_example\test1.mp4
    // C:\Users\floma\CLionProjects\DeliveryBot\video_test_example\test1.mp4
    // E:\Clion Projects\DeliveryBot\video_test_example\test1ShortVer.mp4
    //E:\Clion Projects\DeliveryBot\video_test_example\testPic.jpg

    int rc;
    struct mosquitto * mosq;

    auto neg = mosquitto_lib_init();
    mosq = mosquitto_new("publisher-test", true, NULL);

    rc = mosquitto_connect(mosq, "localhost", 1883, 60);
    if(rc != 0){
        printf("Client could not connect to broker! Error Code: %d\n", rc);
        mosquitto_destroy(mosq);
        return -1;
    }
    printf("We are now connected to the broker!\n");

    mosquitto_publish(mosq, NULL, "test/t1", 6, "Hello", 0, false);

    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);

    mosquitto_lib_cleanup();
    return 0;

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