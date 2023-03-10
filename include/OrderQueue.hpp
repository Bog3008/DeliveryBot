//
// Created by floma on 07.03.2023.
//

#ifndef DELIVERYBOT_ORDERQUEUE_HPP
#define DELIVERYBOT_ORDERQUEUE_HPP
#include <queue>
#include <list>
struct OrderQueue{
private:
    static std::queue<int> queue;
public:
    OrderQueue(){
        queue;
    }

    static bool empty();
    static void pop();
    static void add(int);
};
#endif //DELIVERYBOT_ORDERQUEUE_HPP
