#include <gtest/gtest.h>
#include "loggerFactory.hpp"
#include "logger.hpp"
#include <thread>
#include <atomic>
#include <vector>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class SocketLoggerTest : public ::testing::Test {
protected:
    int serverFd = -1;
    int clientSocket = -1;
    int testPort = 0;
    std::string testHost = "127.0.0.1";
    std::thread serverThread;
    std::atomic<bool> serverRunning{false};
    std::vector<std::string> receivedLogs;

    void SetUp() override {
        serverFd = socket(AF_INET, SOCK_STREAM, 0);
        ASSERT_GE(serverFd, 0);

        int opt = 1;
        setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        sockaddr_in address{};
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = inet_addr(testHost.c_str());
        address.sin_port = 0;

        ASSERT_GE(bind(serverFd, (struct sockaddr*)&address, sizeof(address)), 0);
        ASSERT_GE(listen(serverFd, 1), 0);

        socklen_t addrLen = sizeof(address);
        getsockname(serverFd, (struct sockaddr*)&address, &addrLen);
        testPort = ntohs(address.sin_port);

        serverRunning = true;
        serverThread = std::thread(&SocketLoggerTest::runServer, this);
    }

    void TearDown() override {
        serverRunning = false;
        if (clientSocket >= 0) {
            shutdown(clientSocket, SHUT_RDWR);
            close(clientSocket);
        }
        if (serverFd >= 0) {
            close(serverFd);
        }
        if (serverThread.joinable()) {
            serverThread.join();
        }
    }

    void runServer() {
        sockaddr_in clientAddr{};
        socklen_t clientLen = sizeof(clientAddr);
        
        struct timeval tv{1, 0};
        setsockopt(serverFd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

        clientSocket = accept(serverFd, (struct sockaddr*)&clientAddr, &clientLen);
        if (clientSocket < 0) return;

        char buffer[1024];
        while (serverRunning) {
            memset(buffer, 0, sizeof(buffer));
            int bytesRead = read(clientSocket, buffer, sizeof(buffer) - 1);
            if (bytesRead <= 0) break;

            std::string msg(buffer);
            receivedLogs.push_back(msg);
        }
    }
};

// Создание SocketLogger через LoggerFactory
TEST_F(SocketLoggerTest, CreateSocketLoggerViaFactory) {
    auto logger = LoggerFactory::createSocketLogger(testHost, testPort, LogLevel::WARNING);
    
    ASSERT_NE(logger, nullptr);
    EXPECT_NE(dynamic_cast<SocketLogger*>(logger.get()), nullptr);
    EXPECT_EQ(logger->getDefaultLevel(), LogLevel::WARNING);
}

// Отправка сообщений в сокет с фильтрацией по уровню
TEST_F(SocketLoggerTest, LogsToSocketWithLevelFiltering) {
    auto logger = LoggerFactory::createSocketLogger(testHost, testPort, LogLevel::WARNING);
    ASSERT_NE(logger, nullptr);

    logger->log("Ignored info message", LogLevel::INFO);
    logger->log("Valid warning message", LogLevel::WARNING);
    logger->log("Valid error message", LogLevel::ERROR);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::string allData;
    for (const auto& log : receivedLogs) {
        allData += log;
    }

    EXPECT_EQ(allData.find("Ignored info message"), std::string::npos);
    EXPECT_NE(allData.find("Valid warning message"), std::string::npos);
    EXPECT_NE(allData.find("Valid error message"), std::string::npos);
    EXPECT_NE(allData.find("[WARNING]"), std::string::npos);
    EXPECT_NE(allData.find("[ERROR]"), std::string::npos);
}

// Динамическая смена уровня логирования
TEST_F(SocketLoggerTest, ChangeLogLevelAtRuntime) {
    auto logger = LoggerFactory::createSocketLogger(testHost, testPort, LogLevel::INFO);
    ASSERT_NE(logger, nullptr);

    logger->setLogLevel(LogLevel::ERROR);
    EXPECT_EQ(logger->getDefaultLevel(), LogLevel::ERROR);

    logger->log("Warning after level change", LogLevel::WARNING);
    logger->log("Critical error", LogLevel::ERROR);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::string allData;
    for (const auto& log : receivedLogs) {
        allData += log;
    }

    EXPECT_EQ(allData.find("Warning after level change"), std::string::npos);
    EXPECT_NE(allData.find("Critical error"), std::string::npos);
}