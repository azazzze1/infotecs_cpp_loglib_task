#include "logger/loggerFactory.hpp"
#include "logger/ilogger.hpp"
#include <iostream>

int main(int argc, char** argv)
{
	// Создаем логгер с уровнем INFO по умолчанию
    auto logger = LoggerFactory::createSocketLogger("127.0.0.1", 12345, LogLevel::INFO);

    if (!logger) {
        std::cerr << "Не удалось создать логгер" << std::endl;
        return 1;
    }

    // Записываем логи разных уровней
    logger->log(LogLevel::INFO, "Это информационное сообщение");
    logger->log(LogLevel::WARNING, "Это предупреждение");
    logger->log(LogLevel::ERROR, "Это ошибка");

    // Меняем уровень логирования на ERROR
    logger->setLogLevel(LogLevel::ERROR);

    logger->log(LogLevel::INFO, "[НЕ ДОЛЖЕН ПОЯВИТЬСЯ] Это сообщение уровня INFO (ниже ERROR)");
    logger->log(LogLevel::ERROR, "Это сообщение уровня ERROR (должно появиться)");

    std::cout << "Логи успешно записаны в файл app.log" << std::endl;

	return 0;
}