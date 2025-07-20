#include <gtest/gtest.h>
#include "logger.hpp"
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>
#include <iostream>

class FileLoggerTest : public ::testing::Test {
protected:
    const std::string testFile = "test_log.txt";
    void SetUp() override {
        std::ofstream ofs(testFile, std::ofstream::trunc);
        ofs.close();
    }
};

// Логирование в файл с указанием уровня важности
TEST_F(FileLoggerTest, LogToFileWithCustomLevel) {
    FileLogger logger(testFile, LogLevel::INFO);
    logger.log("Info message", LogLevel::INFO);
    logger.log("Warning message", LogLevel::WARNING);
    logger.log("Error message", LogLevel::ERROR);

    std::ifstream ifs(testFile);
    std::string line;

    std::vector<std::string> levelList = {"[INFO]", "[WARNING]", "[ERROR]"};
    std::vector<std::string> textList = {"Info message", "Warning message", "Error message"}; 
    size_t i = 0; 

    while(std::getline(ifs, line)){
        EXPECT_TRUE(line.find(levelList[i]) != std::string::npos);
        EXPECT_TRUE(line.find(textList[i++]) != std::string::npos);
    }
}

// Логирование без указания класса важности
TEST_F(FileLoggerTest, LogToFileWithDefaultLevel) {
    FileLogger logger(testFile, LogLevel::INFO);
    logger.log("Test message");
    
    std::ifstream ifs(testFile);
    std::string line;
    std::getline(ifs, line);
    
    EXPECT_TRUE(line.find("[INFO]") != std::string::npos);
    EXPECT_TRUE(line.find("Test message") != std::string::npos);
}

// Логирование с уровнем важности меньше уровня по умолчанию 
TEST_F(FileLoggerTest, LogToFileLevelFiltering) {
    FileLogger logger(testFile, LogLevel::ERROR);
    logger.log("Info message", LogLevel::INFO);

    std::ifstream ifs(testFile);
    std::string line;
    std::getline(ifs, line);
    EXPECT_TRUE(line.empty());
}

// Логирование меньшего уровня важности после изменения уровня по умолчанию  
TEST_F(FileLoggerTest, LogFileChangeLogLevel) {
    FileLogger logger(testFile, LogLevel::INFO);
    logger.setLogLevel(LogLevel::ERROR);
    logger.log("Info message", LogLevel::INFO);

    std::ifstream ifs(testFile);
    std::string line;
    std::getline(ifs, line);
    EXPECT_TRUE(line.empty());
}

// Многопоточная запись в файл
TEST_F(FileLoggerTest, MultiThreadedFileLogging) {
    FileLogger logger(testFile, LogLevel::INFO);

    auto logFunc = [&logger](int id) {
        logger.log("Thread " + std::to_string(id));
    };

    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back(logFunc, i);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::ifstream ifs(testFile);
    std::string line;
    int count = 0;
    while (std::getline(ifs, line)) {
        if (line.find("Thread") != std::string::npos) {
            ++count;
        }
    }
    EXPECT_EQ(count, 10);
}
