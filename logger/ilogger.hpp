#ifndef ILOGGER_HPP
#define ILOGGER_HPP

#include <string>
#include <memory>

enum class LogLevel {
    INFO,
    WARNING, 
    ERROR 
};

class ILogger{
    public:
        virtual ~ILogger() = default;
        virtual void log(const std::string& message, LogLevel logLevel) = 0; 
        virtual void setLogLevel(LogLevel level) = 0;
};

#endif 