
#include <iostream>
#include "TgBot.h"
#include "Delbot.hpp"

void controller_start(){
    while(true) {
        Delbot debo;
        try {
            std::string tg_responce = tg_bot_run();
            if (tg_responce != "start") {
                //incorrect message;
                std::cout << "unspecified message" << std::endl;
                return;
            }
            std::cout << tg_responce;
            debo.run();
        }
        catch (std::exception e) {
            std::cout << e.what();
        }
    }
}

int main() {
    controller_start();
}
