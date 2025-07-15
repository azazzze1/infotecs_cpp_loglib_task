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
        void setLogLevel(LogLevel logLevel);

        static std::unique_ptr<ILogger> createFileLogger(const std::string& filename, LogLevel defaultLevel = LogLevel::INFO);
        static std::unique_ptr<ILogger> createSocketLogger(const std::string& host, int port, LogLevel defaultLevel = LogLevel::INFO);

    protected: 
        LogLevel defaultLevel; 
};

#endif 