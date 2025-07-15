#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "ilogger.hpp"
#include <string>
#include <fstream>

class FileLogger : public ILogger{
public:
    FileLogger (const std::string& filename, LogLevel defaultLevel);
    ~FileLogger () override; 

    void log(LogLevel logLevel, const std::string& message) override; 

private:
    std::ofstream file_; 
}

class SocketLogger{}

#endif 