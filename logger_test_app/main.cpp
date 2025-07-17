#include <iostream>

#include "appController.hpp"


int main(int argc, char* argv[]){
    
    appController controller{argc, argv};
    std::string userCommand; 

    while (controller.getFlagForLoop()){
        // здесь будет пользовательский ввод уровня важности и самого сообщения
        std::cin>>userCommand;
        controller.executeCommand(userCommand);
    }

    std::cout<<"КОНЕЦ!"<<std::endl; 

	return 0;
}