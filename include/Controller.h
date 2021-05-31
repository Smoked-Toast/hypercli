#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <iostream>
#include "Parser.h"
#include "Command.h"

class Controller {
    public:
        Controller();
        int execute(int argc, char * argv[]);
    private:
        Parser parser;
        void printUsage(char * name);
};

#endif