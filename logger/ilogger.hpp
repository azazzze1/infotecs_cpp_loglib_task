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
        virtual void log(LogLevel logLevel, const std::string& message) = 0; 
        
        void setLogLevel(LogLevel level) {
            defaultLevel = level;
        }

    protected: 
        LogLevel defaultLevel; 
};

#endif 