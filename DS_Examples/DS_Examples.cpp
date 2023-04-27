#include <iostream>
#include "DS_SynchronousBlockingQueue.h"
#include <iostream>
#include <chrono>
#include <thread>
using namespace std::chrono_literals;

void producer(SynchronousBlockingQueue<int32_t>* queue) 
{
    int32_t elementsToPush = 20;
    while (elementsToPush >= 0) 
    {
        queue->push(elementsToPush--);
        queue->push(elementsToPush--);
        queue->push(elementsToPush--);
        std::this_thread::sleep_for(3000ms);
    }
}
void consumer(SynchronousBlockingQueue<int32_t>* queue) 
{
    int32_t value;
    do 
    {
        value = queue->pop();
        std::this_thread::sleep_for(500ms);
    } while (value != 0);
}

int main() 
{
    SynchronousBlockingQueue<int32_t> queue;
    std::thread thread2(consumer, &queue);
    std::thread thread1(producer, &queue);

    thread1.join();
    thread2.join();
    return 0;
}
