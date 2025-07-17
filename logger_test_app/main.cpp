#include <iostream>
#include <string>
#include <optional>

#include "loggerFactory.hpp"
#include "loggerUtils.hpp"

int main(int argc, char* argv[]){
    
    std::optional<LogLevel> levelOpt = LoggerUtils::stringToLevel(argv[1]);
    if (!levelOpt) {
        std::cerr << "Ошибка: неверный уровень логирования: " << argv[1] << std::endl;
        return 1;
    }

    LogLevel defaultLevel = levelOpt.value();
    std::unique_ptr<ILogger> logger; 

    if(argc==3){
        // Добавить проверку данных
        std::string logFile = argv[2];
        logger = LoggerFactory::createFileLogger(logFile, defaultLevel);
    } else if(argc==4){
        // Добавить проверку данных
        std::string socketAddress = argv[2];
        int socketPort = std::stoi(argv[3]);
        logger = LoggerFactory::createSocketLogger(socketAddress, socketPort, defaultLevel);
    } else{
        std::cout<<"Некорректные данные для подключения!"<<std::endl;
        std::cout<<argv[0]<<" <DEFAULT-LOG-LEVEL>"<<" <relative-logfile-path>"<<std::endl;
        std::cout<<argv[0]<<" <DEFAULT-LOG-LEVEL>"<<" <socket-ip-adress> "<<"<socket-port>"<<std::endl;
        return 0; 
    }

    if (!logger) {
        std::cerr << "Не удалось создать логгер" << std::endl;
        return 1;
    }

    logger->log(LogLevel::INFO, "Это информационное сообщение");
    logger->log(LogLevel::WARNING, "Это предупреждение");
    logger->log(LogLevel::ERROR, "Это ошибка");

    logger->setLogLevel(LogLevel::ERROR);

    logger->log(LogLevel::INFO, "[НЕ ДОЛЖЕН ПОЯВИТЬСЯ] Это сообщение уровня INFO (ниже ERROR)");
    logger->log(LogLevel::ERROR, "Это сообщение уровня ERROR (должно появиться)");

    std::cout << "Логи успешно записаны в файл app.log" << std::endl;

	return 0;
}