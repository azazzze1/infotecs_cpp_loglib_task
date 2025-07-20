#include "logger.hpp"

// Реализация FileLogger 

FileLogger::FileLogger(const std::string& filename, LogLevel defaultLevel) : defaultLevel(defaultLevel) {
    file_.open(filename, std::ios::app);

    if (!file_) std::cerr<<"Ошибка при открытии следующего файла: "<<filename<<std::endl;  
}

FileLogger::~FileLogger(){
    if (file_.is_open()) file_.close();
}

void FileLogger::log(const std::string& message, LogLevel logLevel){
    if(logLevel < defaultLevel) return; 
    std::lock_guard<std::mutex> lock(mtx_); 
    if(!file_.is_open()) return;
    file_<<"["<<LoggerUtils::currentTime()<<"] ["<<LoggerUtils::levelToString(logLevel)<<"] "<<message<<std::endl;
}

void FileLogger::log(const std::string& message){
    log(message, defaultLevel); 
}

void FileLogger::setLogLevel(LogLevel logLevel){
    defaultLevel = logLevel;
}

// Реализация SocketLogger 

SocketLogger::SocketLogger(const std::string& host, int port, LogLevel defaultLevel) : defaultLevel(defaultLevel) {
    defaultLevel = defaultLevel;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        std::cerr<<"Ошибка при создании сокета"<<std::endl;
        return;
    }

    sockaddr_in serv_address{};
    serv_address.sin_family = AF_INET;
    serv_address.sin_port = htons(port);

    if (inet_pton(AF_INET, host.c_str(), &serv_address.sin_addr) <= 0){
        std::cerr<<"Некорректный адрес"<<std::endl;
        close(sockfd);
        sockfd = -1;
        return;
    }

    if (connect(sockfd, (struct sockaddr *)&serv_address, sizeof(serv_address)) < 0){
        std::cerr<<"Ошибка при подключении к сокету"<<std::endl;
        close(sockfd);
        sockfd = -1; 
    }
}

SocketLogger::~SocketLogger(){
    if (sockfd > -1) close(sockfd); 
}

void SocketLogger::log(const std::string& message, LogLevel logLevel) {
    if(logLevel < defaultLevel) return; 

    std::lock_guard<std::mutex> lock(mtx_); 
    
    if (sockfd <= -1) return;

    std::string logMessage = "["+LoggerUtils::currentTime()+"] ["+LoggerUtils::levelToString(logLevel)+"] "+message+"\n";

    send(sockfd, logMessage.c_str(), logMessage.size(), 0); 
}

void SocketLogger::setLogLevel(LogLevel logLevel){
    defaultLevel = logLevel;
}

void SocketLogger::log(const std::string& message){
    log(message, defaultLevel); 
}
