#include "socketStatsCollector.hpp"
#include <iostream>

// Реализация SocketStatsCollector

void SocketStatsCollector::addMessage(const LogMessage& logMessage){
    std::lock_guard<std::mutex> lock(mtx);
    messageHistory.push_back(logMessage); 

    socketStats.messageCount++;
    socketStats.messageLevelCount[logMessage.logLevel]++; 

    size_t logMessageLength = logMessage.text.size() - 2; 
    socketStats.minMessageLength = std::min(socketStats.minMessageLength, logMessageLength);
    socketStats.maxMessageLength = std::max(socketStats.maxMessageLength, logMessageLength);
    socketStats.totalLength += logMessageLength; 

    std::cout<<logMessage.text<<std::endl; 
} 


SocketStats SocketStatsCollector::getSocketStats(){
    std::lock_guard<std::mutex> lock(mtx);
    auto now = std::chrono::system_clock::now();
    auto hourAgo = now - std::chrono::hours(1);

    socketStats.messageForHourCount = std::count_if(messageHistory.begin(), messageHistory.end(), 
        [hourAgo](const LogMessage& msg){
            return msg.date >= hourAgo;
        });
    
    if(socketStats.messageCount != 0)
        socketStats.avgMessageLength = socketStats.totalLength / socketStats.messageCount; 

    
    return socketStats; 
}