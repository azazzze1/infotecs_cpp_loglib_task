#include <iostream>

#include "appController.hpp"

int main(int argc, char* argv[]){
    
    appController controller{argc, argv};

    controller.listenSocket();
    controller.waitForProcessStats();

	return 0;
}