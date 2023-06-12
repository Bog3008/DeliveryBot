//
// Created by floma on 17.05.2023.
//

#ifndef DELIVERYBOT_DELBOT_HPP
#define DELIVERYBOT_DELBOT_HPP
struct Delbot{
    Delbot(){}
    void run();
    static void turn_left(int degrees);
    static void turn_right(int degrees);
    static void move_forward(int meters);
};
void async_sender();
#endif //DELIVERYBOT_DELBOT_HPP
