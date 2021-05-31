#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "Parser.hpp"
#include "Command.hpp"

class Controller {
    public:
        //Constructor
        Controller();

        //Process the cmd line args
        int execute(int argc, char * argv[]);
    private:
        //String parser for cmd line args
        Parser parser;

        //Print usage for main interface
        void printUsage(char * name);

        //Deploy a virtual machine on the local hypervisor
        int deploy(std::string organization, std::string vmid);
};

#endif