#include <gtest/gtest.h>
#include "loggerUtils.hpp"

// Конвертация верного LogLevel из строки в LogLevel
TEST(LoggerUtilsTest, ConvertsStringToLevel_Valid) {
    auto level = LoggerUtils::stringToLevel("INFO");
    ASSERT_TRUE(level.has_value());
    EXPECT_EQ(level.value(), LogLevel::INFO);

    level = LoggerUtils::stringToLevel("WARNING");
    ASSERT_TRUE(level.has_value());
    EXPECT_EQ(level.value(), LogLevel::WARNING);

    level = LoggerUtils::stringToLevel("ERROR");
    ASSERT_TRUE(level.has_value());
    EXPECT_EQ(level.value(), LogLevel::ERROR);
}

// Конвертация неизвестного LogLevel из строки в LogLevel
TEST(LoggerUtilsTest, ConvertsStringToLevel_Invalid) {
    auto level = LoggerUtils::stringToLevel("UNKNOWN");
    EXPECT_FALSE(level.has_value());
}

// Конвертация верного LogLevel из LogLevel в строку 
TEST(LoggerUtilsTest, ConvertsLevelToString_Valid) {
    EXPECT_EQ(LoggerUtils::levelToString(LogLevel::INFO), "INFO");
    EXPECT_EQ(LoggerUtils::levelToString(LogLevel::WARNING), "WARNING");
    EXPECT_EQ(LoggerUtils::levelToString(LogLevel::ERROR), "ERROR");
}

// Конвертация неизвестного LogLevel из LogLevel в строку 
TEST(LoggerUtilsTest, ConvertsLevelToString_Invalid) {
    EXPECT_EQ(LoggerUtils::levelToString(static_cast<LogLevel>(1000)), "UNKNOWN");
}

// Проверка валидации std::optional<LogLevel> с существующим значением
TEST(LoggerUtilsTest, ValidateLogLevel_HasValue) {
    LogLevel outLevel;
    EXPECT_TRUE(LoggerUtils::validateLogLevel(std::optional<LogLevel> (LogLevel::WARNING), outLevel));
    EXPECT_EQ(outLevel, LogLevel::WARNING);
}

// Проверка валидации std::optional<LogLevel> с нулевым указателем
TEST(LoggerUtilsTest, ValidateLogLevel_NoValue) {
    LogLevel outLevel;
    EXPECT_FALSE(LoggerUtils::validateLogLevel(std::optional<LogLevel> (std::nullopt), outLevel));
}