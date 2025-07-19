#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

const int PORT = 12345;
const int BUFFER_SIZE = 1024;

int senderSocket = -1;
int receiverSocket = -1;
std::mutex socketMutex;

void handleClient(int clientSocket, bool isSender) {
    char buffer[BUFFER_SIZE];
    if (isSender) {
        {
            std::unique_lock<std::mutex> lock(socketMutex);
            senderSocket = clientSocket;
            std::cout << "Подключён отправитель логов." << std::endl;
        } // lock выходит из области видимости здесь

        while (true) {
            memset(buffer, 0, BUFFER_SIZE);
            int valread = read(clientSocket, buffer, BUFFER_SIZE - 1);
            if (valread <= 0) {
                std::cerr << "Отправитель отключён." << std::endl;
                break;
            }

            std::cout << "Получен лог: " << buffer << std::endl;

            {
                std::unique_lock<std::mutex> lock(socketMutex);
                if (receiverSocket != -1) {
                    send(receiverSocket, buffer, valread, 0);
                }
            } // lock выходит из области видимости здесь
        }

        {
            std::unique_lock<std::mutex> lock(socketMutex);
            senderSocket = -1;
            close(clientSocket);
        }
    } else {
        {
            std::unique_lock<std::mutex> lock(socketMutex);
            receiverSocket = clientSocket;
            std::cout << "Подключён получатель логов." << std::endl;
        }

        while (true) {
            sleep(1); // Ждём, пока будут данные
        }
    }
}

int main() {
    int server_fd;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 2) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "TCP-сервер запущен на порту " << PORT << ". Ожидание клиентов..." << std::endl;

    bool isSenderNext = true; // Определяем, кто следующий: отправитель или получатель

    while (true) {
        int new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        if (new_socket < 0) {
            perror("accept");
            continue;
        }

        std::thread clientThread(handleClient, new_socket, isSenderNext);
        clientThread.detach();
        isSenderNext = false;
    }

    close(server_fd);
    return 0;
}