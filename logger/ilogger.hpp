#ifndef ILOGGER_HPP
#define ILOGGER_HPP

#include <string>
#include <memory>


/**
* @brief Уровни логирования для классификации сообщений.
*/
enum class LogLevel {
    INFO, ///< Сообщения, носящие информационный характер 
    WARNING, ///< Сообщения, указывающие на возможную ошибку
    ERROR  ///< Сообщения, указывающие на произошедщую ошибку
};

/** 
* @class ILogger
* @brief Интерфейс для систем логирования.
*
* Предоставляет унифицированный способ записи логов, а также возможность изменять
* уровень логирования по умолчанию. 
*/
class ILogger{
    public:
        /**
        * @brief Виртуальный деструктор по умолчанию. 
        */
        virtual ~ILogger() = default;

        /** 
        * @brief Запись сообщения в лог с указанным уровнем важности.
        * @param message Текст сообщения.
        * @param logLevel Уровень важности.
        */
        virtual void log(const std::string& message, LogLevel logLevel) = 0; 

        /**
        * @brief Запись сообщения в лог без указания уровня важности. 
        * @param message Текст сообщения.
        *
        * Уровень важности по умолчанию зависит от реализации метода. 
        */
        virtual void log(const std::string& message) = 0; 

        /**
        * @brief Изменение уровня важности по умолчанию.  
        * @param logLevel Уровень важности. 
        */
        virtual void setLogLevel(LogLevel logLevel) = 0;

        /**
         * @brief Считывание текущего уровня важности по умолчанию.  
         * 
         * @return LogLevel Текущий уровень важности. 
         */
        virtual LogLevel getDefaultLevel() = 0;
};

#endif 