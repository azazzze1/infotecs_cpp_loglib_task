#include <gtest/gtest.h>
#include "loggerFactory.hpp"
#include "logger.hpp"
#include <fstream>
#include <memory>

class LoggerFactoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        testFilename = "test_log.txt";
    }

    void TearDown() override {
        std::remove(testFilename.c_str());
    }

    std::string testFilename;
    std::string testHost;
    int testPort;
};

// Создание FileLogger.  
TEST_F(LoggerFactoryTest, CreateFileLogger) {
    auto logger = LoggerFactory::createFileLogger(testFilename, LogLevel::ERROR);
    
    ASSERT_NE(logger, nullptr);
    EXPECT_NE(dynamic_cast<FileLogger*>(logger.get()), nullptr);
}

// Проверка значения уровня важности по умолчанию.
TEST_F(LoggerFactoryTest, CreateWithCustomLogLevel) {
    auto fileLogger = LoggerFactory::createFileLogger(testFilename, LogLevel::ERROR);
    EXPECT_EQ(fileLogger->getDefaultLevel(), LogLevel::ERROR);
}