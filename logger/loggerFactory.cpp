#include "loggerFactory.hpp"

std::unique_ptr<ILogger> LoggerFactory::createFileLogger(const std::string& filename, LogLevel defaultLevel) {
    return std::make_unique<FileLogger>(filename, defaultLevel);
}

std::unique_ptr<ILogger> LoggerFactory::createSocketLogger(const std::string& host, int port, LogLevel defaultLevel) {
    return std::make_unique<SocketLogger>(host, port, defaultLevel);
}