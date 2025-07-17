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
    std::time_t tTime = std::chrono::system_clock::to_time_t(time);
    std::string fullDate = std::ctime(&tTime);
    if(!fullDate.empty()) fullDate.pop_back();
    return fullDate;              
}

bool LoggerUtils::validateLogLevel(std::optional<LogLevel> optLevel, LogLevel& outLogLevelValue){
    if (!optLevel) {
        std::cerr<<"Ошибка: некорректный уровень логирования"<<std::endl;
        return false;
    }

    outLogLevelValue = optLevel.value();
    return true; 
}
