#include <iostream>

#include "appController.hpp"

int main(int argc, char* argv[]){
    
    appController controller{argc, argv};

    controller.run();

	return 0;
}