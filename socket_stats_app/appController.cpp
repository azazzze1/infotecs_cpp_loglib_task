#include "appController.hpp"

appController::appController(int argc, char* argv[]){
    if(argc!= 5){
        std::cerr<<"\tВведены не все параметры:"<<std::endl;
        std::cout<<"\t"<<argv[0]<<" <socket-ip-adress> "<<"<socket-port> "<<"<N> "<<"<T>"<<std::endl;
        return;
    }

    std::string socketAddress = argv[1];
    int socketPort = std::stoi(argv[2]);

    if (!connectToSocket(socketAddress, socketPort)){
        std::cerr<<"\tЗапустите приложение с верными данными"<<std::endl; 
        return;
    }

    N = std::stoi(argv[3]);
    T = std::stoi(argv[4]);
    flagForWorkLoop = true;  
}

bool appController::connectToSocket(const std::string& socketAddress, int socketPort){
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd < 0){
        std::cerr<<"\tОшибка при создании сокета"<<std::endl;
        return false;
    }

    sockaddr_in serv_address{};
    serv_address.sin_family = AF_INET;
    serv_address.sin_port = htons(socketPort);

    if (inet_pton(AF_INET, socketAddress.c_str(), &serv_address.sin_addr) <= 0){
        std::cerr<<"\tНекорректный адрес"<<std::endl;
        close(socketfd);
        socketfd = -1;
        return false;
    }

    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    setsockopt(socketfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    if (connect(socketfd, (struct sockaddr *)&serv_address, sizeof(serv_address)) < 0){
        std::cerr<<"\tОшибка при подключении к сокету"<<std::endl;
        close(socketfd);
        socketfd = -1;
        return false; 
    }

    return true;
}

void appController::listenSocket(){
    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];

    while(flagForWorkLoop){
        memset(buffer, 0, BUFFER_SIZE);
        int valread = read(socketfd, buffer, BUFFER_SIZE - 1);

        if (valread < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                continue;
            } else {
                std::cerr << "\tОшибка чтения: " << strerror(errno) << std::endl;
                flagForWorkLoop = false;
                break;
            }
        }

        std::string receivedMessage(buffer);
        std::istringstream iss(receivedMessage);
        std::string line;

        while(std::getline(iss,line)){
            LogMessage logMessage;
            if (parseLogMessage(line, logMessage)){
                socketStatsCollector.addMessage(logMessage);
            }
        }
    }
}

bool appController::parseLogMessage(const std::string& line, LogMessage& outLogMessage){
    size_t dateStartPos = line.find('['); 
    size_t dateEndPos = line.find(']');
    size_t levelStartPos = line.find('[', dateEndPos+1);
    size_t levelEndPos = line.find(']', levelStartPos);

    if (dateStartPos == std::string::npos || dateEndPos == std::string::npos ||
        levelStartPos == std::string::npos || levelEndPos == std::string::npos ||
        levelEndPos + 2 >= line.length()){
            std::cerr<<"\tНеправильный формат сообщения"<<std::endl;
            return false;
        }
    
    std::string strLogLevel = line.substr(levelStartPos+1, levelEndPos-levelStartPos-1);

    std::optional<LogLevel> optLevel = LoggerUtils::stringToLevel(strLogLevel);
    LogLevel logLevel;
    if(LoggerUtils::validateLogLevel(optLevel, logLevel)){
        outLogMessage.logLevel = logLevel; 
    }else{
        std::cerr<<"\tНекорректный уровень логирования!"<<std::endl;
        return false;
    }
    
    std::string logText = line.substr(levelEndPos + 2);
    outLogMessage.text = logText;

    std::string strLogDate = line.substr(dateStartPos+1, dateEndPos-dateStartPos-1);

    std::tm tm = {};
    std::istringstream ss(strLogDate);

    ss >> std::get_time(&tm, "%a %b %d %H:%M:%S %Y");

    if (ss.fail()) {
        std::cerr<<"\tНекорректный формат даты в логе!"<<std::endl;
        return false;
    }

    std::time_t time = std::mktime(&tm);
    std::chrono::system_clock::time_point logDate = std::chrono::system_clock::from_time_t(time);

    outLogMessage.date = logDate;

    return true;
}

void appController::showStats(SocketStats currentSocketStats){
    std::cout<<"\t\tСтатистика по количеству сообщений"<<std::endl;
    std::cout<<"\tВсего сообщений: "<<currentSocketStats.messageCount<<std::endl;
    std::cout<<"\tСообщений уровня INFO: "<<currentSocketStats.messageLevelCount[LogLevel::INFO]<<std::endl;
    std::cout<<"\tСообщений уровня WARNING: "<<currentSocketStats.messageLevelCount[LogLevel::WARNING]<<std::endl;
    std::cout<<"\tСообщений уровня ERROR: "<<currentSocketStats.messageLevelCount[LogLevel::ERROR]<<std::endl;
    std::cout<<"\tСообщений за последний час: "<<currentSocketStats.messageForHourCount<<std::endl;
    
    std::cout<<"\t\tСтатистика по длине сообщений"<<std::endl;
    std::cout<<"\tМинимальная длина: "<<currentSocketStats.minMessageLength<<std::endl;
    std::cout<<"\tМаксимальная длина: "<<currentSocketStats.maxMessageLength<<std::endl;
    std::cout<<"\tСредняя длина: "<<currentSocketStats.avgMessageLength<<std::endl;
    std::cout<<std::endl; 
}

void appController::waitForProcessStats(){
    auto lastStatsTime = std::chrono::system_clock::now();
    SocketStats lastStats;

    while(flagForWorkLoop){
        SocketStats newStats = socketStatsCollector.getSocketStats();
        auto currentTime = std::chrono::system_clock::now();

        if(newStats.messageCount - lastStats.messageCount >= N){
            showStats(newStats);
            lastStats = newStats;
            lastStatsTime = currentTime; 
        }

        if(std::chrono::duration_cast<std::chrono::seconds>(currentTime - lastStatsTime).count() >= T){
            if(lastStats.totalLength != newStats.totalLength){
                showStats(newStats);
                lastStatsTime = currentTime;
            }
        }
    }

    std::this_thread::sleep_for(std::chrono::seconds(1)); 
}

appController::~appController(){
    flagForWorkLoop = false;
    if (receiverThread.joinable()) {
        receiverThread.join();
    }
    if (statsThread.joinable()) {
        statsThread.join();
    }
    close(socketfd);
}

void appController::run(){
    receiverThread = std::thread(&appController::listenSocket, this);
    statsThread = std::thread(&appController::waitForProcessStats, this);

    if (receiverThread.joinable()) receiverThread.join();
    if (statsThread.joinable()) statsThread.join();
}