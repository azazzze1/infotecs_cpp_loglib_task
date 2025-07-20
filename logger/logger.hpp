#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "ilogger.hpp"
#include "loggerUtils.hpp"
#include <string>
#include <fstream>
#include <iostream> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <mutex>


/**
 * @class FileLogger
 * @brief Реализация ILogger, записывающая логи в файл.
 *
 * Пример формата записи лога: 
 * [Fri Jul 18 13:29:55 2025] [INFO] Text for test?
 */
class FileLogger : public ILogger{
public:
    /**
     * @brief Конструктор с подключением к файлу логирования. 
     * 
     * @param filename Относительный путь к файлу записи. 
     * @param defaultLevel Уровень важности по умолчанию. 
     *
     * Если файла не существует, то создаётся новый.  
     */
    FileLogger(const std::string& filename, LogLevel defaultLevel);

    /**
     * @brief Деструктор. Закрывает файл при уничтожении объекта. 
     */
    ~FileLogger() override; 

    /** 
    * @brief Логирование в файл с указанным уровнем важности.
    * @param message Текст сообщения.
    * @param logLevel Уровень важности.
    */
    void log(const std::string& message, LogLevel logLevel) override;

    /**
    * @brief Логирование в файл c уровнем важности defaultLevel. 
    * @param message Текст сообщения.
    */
    void log(const std::string& message) override; 

    /**
    * @brief Изменение уровня важности по умолчанию.  
    * @param logLevel Уровень важности. 
    */
    void setLogLevel(LogLevel logLevel) override; 

private:
    LogLevel defaultLevel; ///< Уровень логирования по умолчанию. 
    std::ofstream file_; ///< Поток файла для записи логов. 
    std::mutex mtx_; ///< Защита доступа к файлу при многопоточности. 
};

/**
 * @class SocketLogger
 * @brief Реализация ILogger, отправляющая логи в сокет.
 *
 * Формат записи лога: 
 * [Fri Jul 18 13:29:55 2025] [INFO] Text for test? 
 */
class SocketLogger : public ILogger{
public:
    /**
     * @brief Конструктор с подключением к сокету.
     * 
     * @param host IP-адрес сокета для подключения.
     * @param port PORT сокета для подключения. 
     * @param defaultLevel Уровень логирования по умолчанию. 
     */
    SocketLogger(const std::string& host, int port, LogLevel defaultLevel);

    /**
     * @brief Деструктор. Завершает соединение с сокетом. 
     */
    ~SocketLogger() override; 

    /** 
    * @brief Отправка лога в сокет с указанным уровнем важности.
    * @param message Текст сообщения.
    * @param logLevel Уровень важности.
    */
    void log(const std::string& message, LogLevel logLevel) override;

    /**
    * @brief Отправка лога в сокет c уровнем важности defaultLevel. 
    * @param message Текст сообщения.
    */
    void log(const std::string& message) override; 

    /**
    * @brief Изменение уровня важности по умолчанию.  
    * @param logLevel Уровень важности. 
    */
    void setLogLevel(LogLevel logLevel) override; 

private:
    int sockfd; ///< Дескриптор сокета для поддержки соединения.
    LogLevel defaultLevel; ///< Уровень важности по умолчанию.
    std::mutex mtx_;  ///< Защита доступа к сокету при многопоточности.
};

#endif 