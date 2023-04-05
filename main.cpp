#include "opencv2/opencv.hpp"
#include <iostream>
#include "ObjectsDetector.hpp"
#include <cmath>
#include "Robot.hpp"
#include "OrderQueue.hpp"
#include <stdio.h>
#include <Publisher.h>
#include "TgBot.h"
#include "Data.h"
//cd E:\Clion Projects\DelBotSub\mosquitto\build\client\Debug

void controller_start(){
    try {
        std::string tg_responce = tg_bot_run();
        if (tg_responce != "start") {
            //incorrect message handling;
            std::cout << "unspecified message"<<std::endl;
            return;
        }
        std::cout << tg_responce;
        RobotTest kos_bot(Data::cam);
        kos_bot.do_clean();
    }
    catch(std::exception e){
        std::cout << e.what();
    }
}

int main() {
    controller_start();
    /*
     important info about host, port, topic, cam are in Data.h
     */
}