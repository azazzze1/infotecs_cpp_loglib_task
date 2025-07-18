#include "appController.hpp"

appController::appController(int argc, char* argv[]) : flagForLoop(false){
    std::optional<LogLevel> levelOpt = LoggerUtils::stringToLevel(argv[1]);

    LogLevel defaultLevel;

    if(!LoggerUtils::validateLogLevel(levelOpt, defaultLevel)){
        return;
    } 

    if(argc==3){
        std::string logFile = argv[2];
        logger = LoggerFactory::createFileLogger(logFile, defaultLevel);
    } else if(argc==4){
        std::string socketAddress = argv[2];
        int socketPort = std::stoi(argv[3]);
        logger = LoggerFactory::createSocketLogger(socketAddress, socketPort, defaultLevel);
    } else{
        std::cout<<"Некорректные данные для подключения!"<<std::endl;
        std::cout<<argv[0]<<" <DEFAULT-LOG-LEVEL>"<<" <relative-logfile-path>"<<std::endl;
        std::cout<<argv[0]<<" <DEFAULT-LOG-LEVEL>"<<" <socket-ip-adress> "<<"<socket-port>"<<std::endl;
        return; 
    }

    if (!logger) {
        std::cerr << "Не удалось создать логгер" << std::endl;
        return;
    }

    logThread = std::thread([this] {logQueue.run();});

    flagForLoop = true; 
}

appController::~appController(){
    logQueue.stop();
    if (logThread.joinable()) logThread.join();
}

void appController::log(const std::string& message, const std::string& strLogLevel){
    std::optional<LogLevel> optLevel = LoggerUtils::stringToLevel(strLogLevel);
    LogLevel logLevel;
    if(LoggerUtils::validateLogLevel(optLevel, logLevel)){
        logQueue.addTask([this, message, logLevel] {logger->log(message, logLevel);});
        
    } 
    else{
        std::string fullMessage = strLogLevel + ' ' + message;
        logQueue.addTask([this, fullMessage] {logger->log(fullMessage);});
    }
}

void appController::setLogLevel(const std::string& strLogLevel){
    std::optional<LogLevel> optLevel = LoggerUtils::stringToLevel(strLogLevel);
    LogLevel logLevel;
    if(LoggerUtils::validateLogLevel(optLevel, logLevel))
        logQueue.addTask([this, logLevel] {logger->setLogLevel(logLevel);}); 
    else{
        std::cerr<<"\tНекорректный уровень логирования!"<<std::endl;
        std::cerr<<"\tДоступны следующие уровни логирования: INFO, WARNING, ERROR"<<std::endl; 
    }
}

bool appController::getFlagForLoop(){
    return flagForLoop; 
}

void appController::executeCommand(const std::string& command){
    std::istringstream iss(command);
    std::string commandName;
    iss>>commandName; 

    if(commandName == "exit"){
        std::cout<<"\tЗавершение программы..."<<std::endl;
        flagForLoop = false;
    }else if(commandName == "log"){
        std::string logLevel, message;
        iss>>logLevel;

        if(iss.eof()){
            message = " ";
        }else{
            std::getline(iss >> std::ws, message);
        }

        log(message, logLevel); 
        
    }else if(commandName == "setlevel"){
        std::string logLevel;
        iss>>logLevel;
        setLogLevel(logLevel);
    }else{
        std::cout<<"\tКоманда "<<command<< " не распознана!"<<std::endl;
        std::cout<<"\tДоступные команды: log, setlevel, exit"<<std::endl;
    }

}

void appController::stopApplication(){
    flagForLoop = false;  
}