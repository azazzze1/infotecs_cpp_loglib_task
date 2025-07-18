#include "threadLogQueue.hpp"

void ThreadLogQueue::addTask(std::function<void()> task){
    std::lock_guard<std::mutex> lock(mtx);
    taskQueue.push(task);
    syncVariable.notify_one(); 
}

void ThreadLogQueue::run(){
    while(true){
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(mtx);
            syncVariable.wait(lock, [this] {return !taskQueue.empty() || stopQueueFlag;});
            if (stopQueueFlag && taskQueue.empty()) break;
            task = taskQueue.front();
            taskQueue.pop();
        }
        task(); 
    }
} 

void ThreadLogQueue::stop(){
    std::lock_guard<std::mutex> lock(mtx);
    stopQueueFlag = true;
    syncVariable.notify_one();
}