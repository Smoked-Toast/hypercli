#include "../include/Controller.hpp"
#include <iostream>
#include <unistd.h>

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
     * 2. Configure Guest Network
     *  - create vxlan interface
     *  - create bridge for the vxlan interface
     *  - add fdb entries
     *  - turn on all interfaces
     * 
     * 3. 
     **/

    //TODO
    //Get this data from some data object in storage
    std::vector<char *> argv;

    const char * vmtype = "kvm";
    const char * ram = "4096";
    const char * vcpu = "1";
    const char * ostype = "linux";
    const char * bootdisk = "path=/mnt/dcimages/testimg1/testimg1.qcow2,device=disk";
    const char * configdisk = "path=/mnt/dcimages/testimg1/testimg1-seed.qcow2,device=disk";
    const char * networkconfig = "bridge=br-vxlan0,model=virtio,mac=52:54:00:de:45:59";

    argv.push_back((char *)"/usr/bin/virt-install");
    argv.push_back((char *)"--virt-type");
    argv.push_back((char *)vmtype);
    argv.push_back((char *)"--name");
    argv.push_back((char *)const_cast<char*>(vmid.c_str()));
    argv.push_back((char *)"--ram");
    argv.push_back((char *)ram);
    argv.push_back((char *)"--vcpus");
    argv.push_back((char *)vcpu);
    argv.push_back((char *)"--os-type");
    argv.push_back((char *)ostype);
    argv.push_back((char *)"--disk");
    argv.push_back((char *)bootdisk);
    argv.push_back((char *)"--disk");
    argv.push_back((char *)configdisk);
    argv.push_back((char *)"--import");
    argv.push_back((char *)"--network");
    argv.push_back((char *)networkconfig);
    argv.push_back((char *)"--noautoconsole");
    argv.push_back(NULL);

    execv(argv[0], &argv[0]);
    return 0;
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
