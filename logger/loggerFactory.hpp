#ifndef LOGGER_FACTORY_HPP
#define LOGGER_FACTORY_HPP

#include "ilogger.hpp"
#include "logger.hpp"
#include <memory>

/**
 * @class LoggerFactory 
 * @brief Паттерн проектирования "Factory" для создания объектов-логгеров.
 *   
 * Позволяет создавать разные типы логгеров через статические методы. 
 */
class LoggerFactory {
    public:
        /**
         * @brief Конструктор с запретом создания объектов класса. 
         */
        LoggerFactory() = delete;
        
        /**
         * @brief Создание объекта-логера типа FileLogger. 
         * 
         * @param filename Относительный путь файла. 
         * @param defaultLevel Уровень важности по умолчанию. 
         * @return std::unique_ptr<ILogger> Умный указатель на созданный логгер. 
         */
        static std::unique_ptr<ILogger> createFileLogger(
            const std::string& filename, 
            LogLevel defaultLevel = LogLevel::INFO);
        
        /**
         * @brief Создание объекта-логера типа SocketLogger. 
         * 
         * @param host IP-адрес сокета для подключения.
         * @param port PORT сокета для подключения. 
         * @param defaultLevel Уровень логирования по умолчанию. 
         * @return std::unique_ptr<ILogger> Умный указатель на созданный логгер. 
         */
        static std::unique_ptr<ILogger> createSocketLogger(
            const std::string& host, int port, 
            LogLevel defaultLevel = LogLevel::INFO);
    };

#endif