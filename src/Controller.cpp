#include "../include/Controller.h"

Controller::Controller() : test(1) { }

int Controller::execute(int argc, char * argv[]){
    Command cmd = this->myparser.ParseArgs(argc, argv);
    return 0;
}
