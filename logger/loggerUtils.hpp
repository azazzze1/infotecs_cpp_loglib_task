#ifndef LOGGER_UTILS_HPP
#define LOGGER_UTILS_HPP

#include "ilogger.hpp"
#include <string>
#include <ctime>
#include <chrono>
#include <optional>
#include <iostream>

/**
 * @class LoggerUtils
 * @brief Инструменты, используемые при логировании файлов.
 */
class LoggerUtils{
public:
    /**
     * @brief Конвертация объекта LogLevel в std::string. 
     * 
     * @param logLevel Уровень важности для конвертации. 
     * @return std::string Возвращает строку для вывода в лог. 
     */
    static std::string levelToString(LogLevel logLevel);

    /**
     * @brief Конвертация std::string в объект LogLevel.
     * 
     * @param stringLevel Строка для конвертации. 
     * @return std::optional<LogLevel> Возвращает уровень важности.
     * Если строка не содержит уровень важности, возвращается нулевой указатель в std::optional. 
     */
    static std::optional<LogLevel> stringToLevel(const std::string& stringLevel);

    /**
     * @brief Определение времени создания лога. 
     * 
     * @return std::string Возвращает строку для вывода в лог.
     * Формат даты логирования: Fri Jul 18 13:29:55 2025
     */
    static std::string currentTime();

    /**
     * @brief Проверка: имеется ли в std::optional значение уровня важности. 
     * 
     * @param optLevel Тип std::optional с уровнем важности или нулевым указателем. 
     * @param outLogLevelValue Если уровень важности есть, то он определяется в данную переменную.
     * @return true Уровень важности найден и передан в outLogLevelValue.
     * @return false optLevel содержит нулевой указатель. 
     */
    static bool validateLogLevel(std::optional<LogLevel> optLevel, LogLevel& outLogLevelValue);
};

#endif