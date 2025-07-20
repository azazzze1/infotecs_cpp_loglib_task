
#ifndef SOCKET_STATS_COLLECTOR_HPP
#define SOCKET_STATS_COLLECTOR_HPP

#include <string>
#include <unordered_map>
#include <chrono>
#include <vector>
#include <algorithm>

#include "ilogger.hpp"

/**
 * @struct SocketStats
 * @brief Структура данных для хранения статистики. 
 */
struct SocketStats{
    size_t messageCount = 0; ///< Всего логов принято.
    std::unordered_map<LogLevel, size_t> messageLevelCount; ///< Количество логов по уровню важности.
    size_t messageForHourCount = 0; ///< Количество логов за последний час. 

    size_t minMessageLength = std::numeric_limits<size_t>::max(); ///< Минимальная длина сообщения.
    size_t maxMessageLength = 0; ///< Максимальная длина сообщения.
    size_t totalLength = 0; ///< Общая длина сообщений (необходима для подсчёта средней длины).

    double avgMessageLength = 0.0;  ///< Средняя длина сообщений. 
};

/**
 * @struct LogMessage
 * @brief Структура данных для хранения распарсинного лога. 
 */
struct LogMessage{
    std::chrono::system_clock::time_point date; ///< Точка времени, когда был сделан лог. 
    LogLevel logLevel; ///< Уровень важности. 
    std::string text; ///< Текст сообщения.
};

/**
 * @class SocketStatsCollector
 * @brief Обработка полученных логов для сбора статистики.
 */
class SocketStatsCollector{
public:
    /**
     * @brief Получение сообщения и запись его в историю логов.
     * 
     * @param message Распарсинный лог от сокета.
     * @details При получение сообщения происходит обнолвение статистик:
     * messageCount, messageLevelCount, minMessageLength, maxMessageLength, totalLength. 
     * @note Таким образом не приходится просматривать историю логов для изменения этих статистик. 
     */
    void addMessage(const LogMessage& message);

    /**
     * @brief Вызов актуальной статистики.
     * 
     * @return SocketStats Возвращает структуру SocketStats с актуальными статистиками.
     * @details Обновляет статистики avgMessageLength и messageForHourCount. 
     */
    SocketStats getSocketStats();

private:
    SocketStats socketStats; ///< Хранит актуальную статистику при её вызове.
    std::vector<LogMessage> messageHistory; ///< История полученных логов.
 };

#endif
