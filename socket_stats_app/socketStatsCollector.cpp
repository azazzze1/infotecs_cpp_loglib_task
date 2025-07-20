#include "socketStatsCollector.hpp"

// Реализация SocketStatsCollector

void SocketStatsCollector::addMessage(const LogMessage& logMessage){
    std::lock_guard<std::mutex> lock(mtx);
    messageHistory.push_back(logMessage); 

    socketStats.messageCount++;
    socketStats.messageLevelCount[logMessage.logLevel]++; 

    size_t logMessageLength = logMessage.text.size(); 
    socketStats.minMessageLength = std::min(socketStats.minMessageLength, logMessageLength);
    socketStats.maxMessageLength = std::max(socketStats.maxMessageLength, logMessageLength);
    socketStats.totalLength += logMessageLength; 
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
    
    // Учитываем пробел и перевод строки в найденных предложениях
    socketStats.minMessageLength-=3;
    return socketStats; 
}