#include <gtest/gtest.h>
#include "socketStatsCollector.hpp"
#include <thread>
#include <vector>
#include <chrono>

class SocketStatsCollectorTest : public ::testing::Test {
protected:
    SocketStatsCollector collector;
};


// Подсчёт уровней логирования
TEST_F(SocketStatsCollectorTest, CountsMessagesByLevel) {
    auto now = std::chrono::system_clock::now();

    collector.addMessage({now, LogLevel::INFO, "Info msg"});
    collector.addMessage({now, LogLevel::INFO, "Another info msg"});
    collector.addMessage({now, LogLevel::WARNING, "Warn msg"});
    collector.addMessage({now, LogLevel::ERROR, "Error msg"});

    SocketStats stats = collector.getSocketStats();
    EXPECT_EQ(stats.messageCount, 4);
    EXPECT_EQ(stats.messageLevelCount[LogLevel::INFO], 2);
    EXPECT_EQ(stats.messageLevelCount[LogLevel::WARNING], 1);
    EXPECT_EQ(stats.messageLevelCount[LogLevel::ERROR], 1);
}

// Подсчёт метрик длины сообщений (min, max, avg)
TEST_F(SocketStatsCollectorTest, CalculatesLengthMetricsCorrectly) {
    auto now = std::chrono::system_clock::now();

    collector.addMessage({now, LogLevel::INFO, "12345"});       
    collector.addMessage({now, LogLevel::WARNING, "1234567890"}); 
    collector.addMessage({now, LogLevel::ERROR, "123456789012345"}); 

    SocketStats stats = collector.getSocketStats();
    EXPECT_EQ(stats.minMessageLength, 5);
    EXPECT_EQ(stats.maxMessageLength, 15);
    EXPECT_DOUBLE_EQ(stats.avgMessageLength, 10.0);
    EXPECT_EQ(stats.totalLength, 30);
}

// Фильтрация сообщений за скользящий час
TEST_F(SocketStatsCollectorTest, FiltersMessagesOlderThanOneHour) {
    auto now = std::chrono::system_clock::now();
    auto twoHoursAgo = now - std::chrono::hours(2);
    auto thirtyMinsAgo = now - std::chrono::minutes(30);

    collector.addMessage({twoHoursAgo, LogLevel::ERROR, "Old message"});
    collector.addMessage({thirtyMinsAgo, LogLevel::INFO, "Recent message 1"});
    collector.addMessage({now, LogLevel::INFO, "Recent message 2"});

    SocketStats stats = collector.getSocketStats();
    EXPECT_EQ(stats.messageCount, 3);
    EXPECT_EQ(stats.messageForHourCount, 2);
}

// Использование многопоточности для добавления сообщений 
TEST_F(SocketStatsCollectorTest, ConcurrentAddMessageSafety) {
    auto now = std::chrono::system_clock::now();
    const int numThreads = 8;
    const int messagesPerThread = 100;

    auto worker = [&]() {
        for (int i = 0; i < messagesPerThread; ++i) {
            collector.addMessage({now, LogLevel::INFO, "Thread message"});
        }
    };

    std::vector<std::thread> threads;
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(worker);
    }

    for (auto& t : threads) {
        t.join();
    }

    SocketStats stats = collector.getSocketStats();
    EXPECT_EQ(stats.messageCount, numThreads * messagesPerThread);
}