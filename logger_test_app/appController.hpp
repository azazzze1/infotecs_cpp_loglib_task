#ifndef APP_CONTROLLER_HPP
#define APP_CONTROLLER_HPP

#include <iostream>
#include <string>
#include <optional>

#include "loggerFactory.hpp"
#include "loggerUtils.hpp"
#include "threadLogQueue.hpp"

class appController{
public:
    appController(int argc, char* argv[]);

    void log(const std::string& message, const std::string& strLogLevel);
    void setLogLevel(const std::string& strLogLevel);  

    void executeCommand(const std::string& command);

    void stopApplication();

    bool getFlagForLoop(); 

private:
    std::unique_ptr<ILogger> logger;
    bool flagForLoop; 
};

#endif