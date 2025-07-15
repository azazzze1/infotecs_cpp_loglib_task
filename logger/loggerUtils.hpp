#ifndef LOGGER_UTILS_HPP
#define LOGGER_UTILS_HPP

#include "ilogger.hpp"
#include <string>
#include <ctime>
#include <chrono>
#include <optional>


class LoggerUtils{
public:
    static std::string levelToString(LogLevel logLevel);
    static std::optional<LogLevel> stringToLevel(const std::string& stringLevel);
    static std::string currentTime();
};

#endif