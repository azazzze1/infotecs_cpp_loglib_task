#ifndef SOCKET_APP_CONTROLLER_HPP
#define SOCKET_APP_CONTROLLER_HPP

#include "socketStatsCollector.hpp"
#include "loggerFactory.hpp"
#include "loggerUtils.hpp"
#include <iostream>
#include <cstring>
#include <iomanip>
#include <thread>

/**
 * @class AppControllerStats
 * @brief Контроллер, отвечающий за жизненный цикл приложения сбора статистики логов.
 * 
 * Обеспечивает подключение к сокету и его прослушивание, парсинг логов из сокета,
 * вывод статистики каждые N сообщений или T секунд. 
 */
class AppControllerStats{
public:
    /**
     * @brief Конструктор, обработывающий входные параметры приложения и вызывающий подключение к сокету. 
     * 
     * @param argc Количество параметров приложения.
     * @param argv Параметры приложения.
     * 
     * @note Запускает прослушивание сокета и вывод статистики в двух разных потоках.
     */
    AppControllerStats(int argc, char* argv[]);

    /**
     * @brief Подключение к сокету по IP-адресу и порту. 
     * 
     * @param socketAddress IP-адрес сокета.
     * @param socketPort Порт сокета. 
     * @return true Подключение к сокету прошло успешно. 
     * @return false Не удалось подключиться к сокету. 
     */
    bool connectToSocket(const std::string& socketAddress, int socketPort);
    
    /**
     * @brief Прослушивание сокета на наличие новых сообщений и отправка их на обработку. 
     */
    void listenSocket();

    /**
     * @brief Парсинг полученных от сокета логов.
     * 
     * @param line Строка, полученная из сокета.
     * @param outLogMessage Выходной лог типа структуры LogMessage, содержащий данные из лога. 
     * @return true Парсинг прошёл без ошибок и был записан в outLogMessage. 
     * @return false Во время парсинга произошла ошибка.
     *
     * @details Правильный формат лога: [ДАТА] [УРОВЕНЬ ЛОГИРОВАНИЯ] ТЕКСТ
                Формат даты: [ДеньНедели Месяц День Часы:Минуты:Секунды Год]
     */
    bool parseLogMessage(const std::string& line, LogMessage& outLogMessage);

    /**
     * @brief Содержит форматированный вывод статистики. 
     * 
     * @param socketStats Класс SocketStats для обработки и демонстрации статистики.
     */
    void showStats(SocketStats socketStats);

    /**
     * @brief Ожидает для вывода статистики N сообщений или T секунд, если есть новые логи.
     */
    void waitForProcessStats(); 

    /**
     * @brief Запускает работу waitForProcessStats() и listenSocket() в двух потоках. 
     * 
     */
    void run();

    /**
     * @brief Деструтор, закрывающий работающие потоки.  
     */
    ~AppControllerStats();

private:
    SocketStatsCollector socketStatsCollector; ///< Собирает логи и обрабатывает их статистику.
    bool flagForWorkLoop = false; ///< Флаг работы приложения. 
    size_t N; ///< Количество логов через которое надо вывести статистику.
    size_t T; ///< Количество секунд через которое надо вывести статистику, если есть новые логи от сокета. 
    int socketfd; ///< Дескриптор сокета для получения логов.
    std::thread receiverThread; ///< Поток для прослушивания сообщений из сокета. 
    std::thread statsThread; ///< Поток для вывода статистики. 
}; 

#endif
