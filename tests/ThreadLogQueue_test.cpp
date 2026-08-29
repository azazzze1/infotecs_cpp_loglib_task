#include <gtest/gtest.h>
#include "threadLogQueue.hpp"
#include <thread>
#include <atomic>
#include <vector>
#include <chrono>

class ThreadLogQueueTest : public ::testing::Test {
protected:
    ThreadLogQueue queue;
    std::thread workerThread;

    void TearDown() override {
        queue.stop();
        if (workerThread.joinable()) {
            workerThread.join();
        }
    }
};

// Проверка выполнения последовательных задач
TEST_F(ThreadLogQueueTest, ExecutesQueuedTasks) {
    std::atomic<int> counter{0};

    workerThread = std::thread(&ThreadLogQueue::run, &queue);

    queue.addTask([&counter]() { counter += 10; });
    queue.addTask([&counter]() { counter += 20; });

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    EXPECT_EQ(counter.load(), 30);
}

// Проверка обработки всех оставшихся задач при остановке
TEST_F(ThreadLogQueueTest, ProcessesRemainingTasksBeforeStop) {
    std::atomic<int> executedTasks{0};
    const int totalTasks = 100;

    for (int i = 0; i < totalTasks; ++i) {
        queue.addTask([&executedTasks]() {
            executedTasks++;
        });
    }

    workerThread = std::thread(&ThreadLogQueue::run, &queue);
    
    queue.stop();
    workerThread.join();

    EXPECT_EQ(executedTasks.load(), totalTasks);
}

// Добавление задач из нескольких потоков 
TEST_F(ThreadLogQueueTest, ConcurrentTaskInsertion) {
    std::atomic<int> completedTasks{0};
    const int numProducers = 4;
    const int tasksPerProducer = 250;

    workerThread = std::thread(&ThreadLogQueue::run, &queue);

    auto producer = [&]() {
        for (int i = 0; i < tasksPerProducer; ++i) {
            queue.addTask([&completedTasks]() {
                completedTasks++;
            });
        }
    };

    std::vector<std::thread> producers;
    for (int i = 0; i < numProducers; ++i) {
        producers.emplace_back(producer);
    }

    for (auto& p : producers) {
        p.join();
    }

    queue.stop();
    workerThread.join();

    EXPECT_EQ(completedTasks.load(), numProducers * tasksPerProducer);
}