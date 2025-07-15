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

class FileLogger : public ILogger{
public:
    FileLogger(const std::string& filename, LogLevel defaultLevel);
    ~FileLogger() override; 

    void log(LogLevel logLevel, const std::string& message) override; 

private:
    std::ofstream file_; 
};

class SocketLogger : public ILogger{
public:
    SocketLogger(const std::string& host, int port, LogLevel defaultLevel);
    ~SocketLogger() override; 

    void log(LogLevel logLevel, const std::string& message) override; 

private:
    int sockfd; 
};

#endif 