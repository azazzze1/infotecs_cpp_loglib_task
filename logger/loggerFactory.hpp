#ifndef LOGGER_UTILS_HPP
#define LOGGER_UTILS_HPP

#include "ilogger.hpp"
#include "logger.hpp"
#include <memory>

class LoggerFactory {
    public:
        virtual ~LoggerFactory() = default;
        
        static std::unique_ptr<ILogger> createFileLogger(
            const std::string& filename, 
            LogLevel defaultLevel = LogLevel::INFO);
            
        static std::unique_ptr<ILogger> createSocketLogger(
            const std::string& host, int port, 
            LogLevel defaultLevel = LogLevel::INFO);
    };

#endif