#include "socketStatsCollector.hpp"

// Реализация SocketStatsCollector

void SocketStatsCollector::addMessage(const LogMessage& logMessage){
    messageHistory.push_back(logMessage); 

    socketStats.messageCount++;
    socketStats.messageLevelCount[logMessage.logLevel]++; 

    size_t logMessageLength = logMessage.text.length(); 
    socketStats.minMessageLength = std::min(socketStats.minMessageLength, logMessageLength);
    socketStats.maxMessageLength = std::max(socketStats.maxMessageLength, logMessageLength);
    socketStats.totalLength += logMessageLength; 
} 


SocketStats SocketStatsCollector::getSocketStats(){
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