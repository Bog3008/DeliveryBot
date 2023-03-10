//
// Created by Bogdan on 10.03.2023.
//

#include "OrderQueue.hpp"
std::queue<int> OrderQueue::queue;

bool OrderQueue::empty() {
    return queue.empty();
}
void OrderQueue::pop(){
    queue.pop();
}
void OrderQueue::add(int a){
    queue.push(a);
}