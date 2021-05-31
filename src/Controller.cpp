#include "../include/Controller.hpp"
#include <iostream>

Controller::Controller() { }

void Controller::printUsage(char * name){
    printf("%s --action [info|deploy|destroy|updatefdb] --organization <org> --vmid <vmid>\n", name);
}

int Controller::deploy(std::string organization, std::string vmid){
    /** 
     * TODO
     * 1. Get VM deployment information
     *  - vxlan group number 
     *  - ip addresses of router and other hypervisors on the same vxlan group
     *  - vcpu's
     *  - ram
     * 
     * 2.  
     **/
    
    
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
        // use stdlib instead
        std::cout << "Error: Bad input for Organization." << std::endl;
        
        return EXIT_FAILURE;
    }
    if (cmd.vmid.length() == 0){
        // TODO
        // Log error
        // use stdlib instead
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
        // TODO
    }
    else if (cmd.action == DEPLOY){
        // TODO
        this->deploy(cmd.organization, cmd.vmid);
    }
    else if (cmd.action == DESTROY){
        // TODO
    }
    else if (cmd.action == UPDATEFDB){
        // TODO
    }
    return EXIT_SUCCESS;
}
