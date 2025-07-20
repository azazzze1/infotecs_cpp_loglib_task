#ifndef THREAD_LOG_QUEUE_HPP
#define THREAD_LOG_QUEUE_HPP

#include <functional>
#include <queue>
#include <thread>
#include <condition_variable>

/**
 * @class ThreadLogQueue
 * @brief Потокобезопасная очередь задач для асинхронного логирования
 * 
 * Реализует механизм producer-consumer для выполнения задач логирования в отдельном потоке.
 * Гарантирует потокобезопасное добавление и выполнение задач.
 */
class ThreadLogQueue{
public:
    /**
     * @brief Добавляет задачу в очередь.
     * 
     * @param task Фунция или участок кода для выполнения. 
     */
    void addTask(std::function<void()> task);

    /**
     * @brief Запускает работу очереди в фоновом режиме. 
     */
    void run(); 

    /**
     * @brief Останавливает работу потока.
     */
    void stop();

private:
    std::queue<std::function<void()>> taskQueue; ///< Очередь задач по принципу FIFO. 
    std::mutex mtx; ///< Мьютекс для обеспечения синхронизации выполнения задач. 
    std::condition_variable syncVariable; ///< Условная переменная для синхронизации при поступлении новых задач.   
    bool stopQueueFlag = false; ///< Флаг для работы программы.  
};


#endif


