#include "loggerUtils.hpp"

std::optional<LogLevel> LoggerUtils::stringToLevel(const std::string& stringLevel){
    if (stringLevel == "INFO")  return LogLevel::INFO;
    else if (stringLevel == "WARNING") return LogLevel::WARNING;
    else if (stringLevel == "ERROR") return LogLevel::ERROR;
    
    return std::nullopt;
}

std::string LoggerUtils::levelToString(LogLevel logLevel){
    switch(logLevel){
        case LogLevel::INFO : return "INFO";
        case LogLevel::WARNING : return "WARNING";
        case LogLevel::ERROR : return "ERROR";
        default : return "UNKNOWN";
    }
}

std::string LoggerUtils::currentTime(){
    auto time = std::chrono::system_clock::now();
    std::time_t createLogTime = std::chrono::system_clock::to_time_t(time);
    return std::ctime(&createLogTime);              
}