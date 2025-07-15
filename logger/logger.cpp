#include "ilogger.hpp"
#include "logger.hpp"

#include <iostream> 
#include <string>


FileLogger::FileLogger(const std::string& filename, LogLevel defaultLevel) {
    defaultLevel = defaultLevel; 
    file_.open(filename, std::ios::app);

    if (!file_) std::cerr<<"Ошибка при открытии следующего файла: "<<filename<<std::endl;  
}

FileLogger::~FileLogger(){
    if (file_.is_open()) file_.close();
}

void FileLogger::log(LogLevel logLevel, const std::string& message){
    if(logLevel < defaultLevel) return; 
    
    file_<<"["<<LoggerUtils::currentTime()<<"] ["<<LoggerUtils::levelToString(logLevel)<<"] ["<<message<<"]"<<std::endl;
}