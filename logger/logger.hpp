#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "ilogger.hpp"
#include "loggerUtils.hpp"
#include <string>
#include <fstream>
#include <iostream> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <mutex>

class FileLogger : public ILogger{
public:
    FileLogger(const std::string& filename, LogLevel defaultLevel);
    ~FileLogger() override; 

    void log(const std::string& message, LogLevel logLevel) override;
    void log(const std::string& message) override; 
    
    void setLogLevel(LogLevel logLevel) override; 

private:
    LogLevel defaultLevel; 
    std::ofstream file_; 
    std::mutex mtx_; 
};

class SocketLogger : public ILogger{
public:
    SocketLogger(const std::string& host, int port, LogLevel defaultLevel);
    ~SocketLogger() override; 

    void log(const std::string& message, LogLevel logLevel) override;
    void log(const std::string& message) override; 

    void setLogLevel(LogLevel logLevel) override; 

private:
    int sockfd;
    LogLevel defaultLevel; 
    std::mutex mtx_; 
};

#endif 