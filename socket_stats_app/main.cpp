#include <iostream>

#include "appControllerStats.hpp"

int main(int argc, char* argv[]){
    
    AppControllerStats controller{argc, argv};

    controller.run();

	return 0;
}