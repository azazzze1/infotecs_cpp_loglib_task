#ifndef SOCKET_APP_CONTROLLER_HPP
#define SOCKET_APP_CONTROLLER_HPP

#include "socketStatsCollector.hpp"
#include "loggerFactory.hpp"
#include "loggerUtils.hpp"
#include <iostream>
#include <cstring>
#include <iomanip>
#include <thread>

class appController{
public:
    appController(int argc, char* argv[]);

    bool connectToSocket(const std::string& socketAddress, int socketPort); 
    void listenSocket();
    bool parseLogMessage(const std::string& line, LogMessage& outLogMessage);

    void showStats(SocketStats socketStats);
    void waitForProcessStats(); 

    void run();

    ~appController();

private:
    SocketStatsCollector socketStatsCollector;
    bool flagForWorkLoop = false;
    size_t N;
    size_t T;
    int socketfd; 
    std::thread receiverThread;
    std::thread statsThread;
}; 

#endif
