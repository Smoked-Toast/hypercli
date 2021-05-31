#include "../include/Controller.h"

Controller::Controller() { }

void Controller::printUsage(char * name){
    printf("%s --action [info|deploy|destroy|updatefdb] --organization <org> --vmid <vmid>\n", name);
}

int Controller::execute(int argc, char * argv[]){
    if (argc == 1){
        this->printUsage(argv[0]);
        return EXIT_SUCCESS;
    }
    Command cmd = this->parser.ParseArgs(argc, argv);

    if (cmd.action == USAGE){
        this->printUsage(argv[0]);
        return EXIT_SUCCESS;
    }
    
    if (cmd.organization.length() == 0){
        // TODO
        // Log error
        std::cout << "Error: Bad input for Organization." << std::endl;
        
        return EXIT_FAILURE;
    }
    if (cmd.vmid.length() == 0){
        // TODO
        // Log error
        std::cout << "Error: Bad input for vmid." << std::endl;
        
        return EXIT_FAILURE;
    }


    // If error
    if (cmd.action == ERROR_ARGS){
        // TODO
        // Log error
        

        return EXIT_FAILURE;
    }
    else if (cmd.action == INFO){

    }
    return EXIT_SUCCESS;
}
