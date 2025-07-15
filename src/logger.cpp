#include "ilogger.hpp"

#include <iostream> 
#include <string>


FileLogger::FileLogger (const std::string& filename, LogLevel defaultLevel) : ILogger(), defaultLevel(defaultLevel){
    file_.open(filename, std::ios::app)

    if (!file_) std::cerr<<"Ошибка при открытии следующего файла: "<<filename<<std::endl;  
}

FileLogger::~FileLogger (){
    if (file_.is_open()) file_.close();
}