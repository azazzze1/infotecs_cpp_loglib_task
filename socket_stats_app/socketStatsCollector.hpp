
#ifndef SOCKET_STATS_COLLECTOR_HPP
#define SOCKET_STATS_COLLECTOR_HPP

#include <string>
#include <unordered_map>
#include <chrono>
#include <vector>
#include <algorithm>

#include "ilogger.hpp"

struct SocketStats{
    size_t messageCount = 0;
    std::unordered_map<LogLevel, size_t> messageLevelCount;
    size_t messageForHourCount = 0;

    size_t minMessageLength = std::numeric_limits<size_t>::max();
    size_t maxMessageLength = 0;
    size_t totalLength = 0;

    double avgMessageLength = 0.0; 
};

struct LogMessage{
    std::chrono::system_clock::time_point date; 
    LogLevel logLevel;
    std::string text;
};

class SocketStatsCollector{
public:
    void addMessage(const LogMessage& message);
    SocketStats getSocketStats();

private:
    SocketStats socketStats;
    std::vector<LogMessage> messageHistory; 
 };

#endif
