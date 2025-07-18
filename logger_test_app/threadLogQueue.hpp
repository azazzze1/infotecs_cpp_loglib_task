#ifndef THREAD_LOG_QUEUE_HPP
#define THREAD_LOG_QUEUE_HPP

#include <functional>
#include <queue>
#include <thread>
#include <condition_variable>

class ThreadLogQueue{
public:
    void addTask(std::function<void()> task);
    void run(); 
    void stop();

private:
    std::queue<std::function<void()>> taskQueue;
    std::mutex mtx;
    std::condition_variable syncVariable;
    bool stopQueueFlag = false; 
};


#endif


