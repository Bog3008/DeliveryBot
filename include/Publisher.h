//
// Created by Bogdan on 19.03.2023.
//

#ifndef DELIVERYBOT_PUBLISHER_H
#define DELIVERYBOT_PUBLISHER_H

#include <mosquitto.h>
#include<stdio.h>
#include<string>
#include<stdexcept>
#include<iostream>
#include <stdlib.h>
struct MPublisher{
private:
    struct mosquitto * mosq;
public:
    MPublisher(const char * host, int port);
    ~MPublisher();
    void publish(std::string);
};


#endif //DELIVERYBOT_PUBLISHER_H
