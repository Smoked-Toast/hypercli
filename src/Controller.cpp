#include "../include/Controller.h"

Controller::Controller() : test(1) { }

int Controller::execute(int argc, char * argv[]){
    Command cmd = this->myparser.ParseArgs(argc, argv);
    std::cout << cmd.action << " " << cmd.organization << " " << cmd.vmid << std::endl;
    return 0;
}
