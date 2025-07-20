#ifndef APP_CONTROLLER_HPP
#define APP_CONTROLLER_HPP

#include <iostream>
#include <string>
#include <optional>
#include <sstream>

#include "loggerFactory.hpp"
#include "loggerUtils.hpp"
#include "threadLogQueue.hpp"


/**
 * @class appController
 * @brief Контроллер, отвечающий за жизненный цикл приложения.
 *
 * Обеспечивает подключение к файлу или сокету для отправки логов,
 * обработку пользовательских команд.  
 */
class appController{
public:
    /**
     * @brief Конструктор, который в зависимости от количества параметров создаёт нужный тип логгера logger и запускает работу программы. 
     * 
     * @param argc Количество параметров приложения. 
     * @param argv Параметры приложения.
     * 
     * @details Ожидаемые параметры для ввода: 
     * 1) <Уровень важности по умолчанию> <Относительный путь файла> -- для FileLogger.
     * 2) <Уровень важности по умолчанию> <IP-адрес сокета> <Порт сокета> -- для SocketLogger.
     * @note Также инициализирует поток logThread для безопасной обработки логов в очереди logQueue при многопоточном режиме.
     */
    appController(int argc, char* argv[]);

    /**
     * @brief Деструктор. Завершает поток logThread для очереди обработки. 
     */
    ~appController(); 

    /**
     * @brief Логирование сообщения с указанным уровнем важности. 
     * 
     * @param message Текст сообщения.
     * @param strLogLevel Уровень важности с типом "строка". 
     */
    void log(const std::string& message, const std::string& strLogLevel);

    /**
     * @brief Устанавливает новый уровень важности по умолчанию. 
     * 
     * @param strLogLevel Уровень важности с типом "строка".
     */
    void setLogLevel(const std::string& strLogLevel);  
    
    /**
     * @brief Обработка пользовательских команд из потока ввода.
     * 
     * @param command Команда для исполнения.
     * 
     * @details Поддерживаемые команды:
     * 1) "log" -- поддерживает ввод с указанием уровня важности и без (с уровнем по умолчанию).
     * 2) "setlevel" -- изменения уровня важности по умолчанию.
     * 3) "exit" -- завершение работы программы.
     *
     * @example Пример команд: log ERROR Произошла критическая ошибка!
     *                         setlevel WARNING  
     */
    void executeCommand(const std::string& command);

    /**
     * @brief Завершение работы программы.
     * @note Устанавливает flagForLoop в значение false.  
     */
    void stopApplication();

    /**
     * @brief Возвращает флаг работы программы. 
     * 
     * @return true Программа работает.
     * @return false Завершение работы программы. 
     */
    bool getFlagForLoop(); 

private:
    std::unique_ptr<ILogger> logger; ///< Используемый логгер 
    bool flagForLoop; ///< Флаг работы программы. 
    ThreadLogQueue logQueue; ///< Очередь из задач для многопоточного режима работы.
    std::thread logThread; ///< Поток для обработки задач очереди в фоновом режиме. 
};

#endif