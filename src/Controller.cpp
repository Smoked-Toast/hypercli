#include "../include/Controller.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

// Function prototype.
int deploy(Command cmd);

Controller::Controller() { }

int Controller::Sandbox(Command cmd, int (*f)(Command)) {
    pid_t pid;
    int status;
    if ((pid = fork()) == 0){
        //Call the input function pointer
        f(cmd);
        return EXIT_SUCCESS;
    } 
    else if (pid == -1){
        printf("Error: fork()\n");
        return EXIT_FAILURE;
    }
    else {
        if (waitpid(pid, &status, 0) > 0) {
            
            if (WIFEXITED(status) && !WEXITSTATUS(status)){
                printf("program execution successful\n");\
                return EXIT_SUCCESS;
            }
            else if (WIFEXITED(status) && WEXITSTATUS(status)) {
                if (WEXITSTATUS(status) == 127) {
                    printf("execv failed\n");
                    return EXIT_FAILURE;
                }
                else {
                    printf("program terminated normally, but returned a non-zero status\n");   
                    return EXIT_FAILURE;             
                }
            }
            else {
                printf("program didn't terminate normally\n");  
                return EXIT_FAILURE;  
            }     
        } 
        else {
            printf("waitpid() failed\n");
            return EXIT_FAILURE;
        }
    }
}

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

    int retval;
    retval = EXIT_SUCCESS;
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
        int (*f)(Command) = deploy;
        this->Sandbox(cmd, f);
    }
    else if (cmd.action == DESTROY){
        // TODO
    }
    else if (cmd.action == UPDATEFDB){
        // TODO
    }
    return retval;
}

int deploy(Command cmd){
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
     **/

    //TODO
    //Get this data from some data object in remote storage
    const char * vmtype = "kvm";
    const char * ram = "4096";
    const char * vcpu = "1";
    const char * ostype = "linux";
    const char * bootdisk = "path=/mnt/dcimages/testimg1/testimg1.qcow2,device=disk";
    const char * configdisk = "path=/mnt/dcimages/testimg1/testimg1-seed.qcow2,device=disk";
    const char * networkconfig = "bridge=br-vxlan0,model=virtio,mac=52:54:00:de:45:59";

    std::vector<char *> spawnvm;
    spawnvm.push_back((char *)"/usr/bin/virt-install");
    spawnvm.push_back((char *)"--virt-type");
    spawnvm.push_back((char *)vmtype);
    spawnvm.push_back((char *)"--name");
    spawnvm.push_back((char *)const_cast<char*>(cmd.vmid.c_str()));
    spawnvm.push_back((char *)"--ram");
    spawnvm.push_back((char *)ram);
    spawnvm.push_back((char *)"--vcpus");
    spawnvm.push_back((char *)vcpu);
    spawnvm.push_back((char *)"--os-type");
    spawnvm.push_back((char *)ostype);
    spawnvm.push_back((char *)"--disk");
    spawnvm.push_back((char *)bootdisk);
    spawnvm.push_back((char *)"--disk");
    spawnvm.push_back((char *)configdisk);
    spawnvm.push_back((char *)"--import");
    spawnvm.push_back((char *)"--network");
    spawnvm.push_back((char *)networkconfig);
    spawnvm.push_back((char *)"--noautoconsole");
    spawnvm.push_back(NULL);

    execv(spawnvm[0], &spawnvm[0]);

    return EXIT_SUCCESS;
}