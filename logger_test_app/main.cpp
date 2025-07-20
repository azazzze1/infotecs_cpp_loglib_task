#include <iostream>

#include "appControllerLogger.hpp"


int main(int argc, char* argv[]){
    
    AppControllerLogger controller{argc, argv};
    std::string userCommand; 

    while (controller.getFlagForLoop()){
        std::cout<<'~';
        std::getline(std::cin, userCommand);
        controller.executeCommand(userCommand);
    }

    std::cout<<"КОНЕЦ!"<<std::endl; 

	return 0;
}