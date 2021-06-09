#include "../include/Controller.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

// Function prototype.
int deploy(void * vm);

Controller::Controller() { }

int Controller::Sandbox(char * argv[]) {
    pid_t pid;
    int status;
    if ((pid = fork()) == 0){
        //Call the input function pointer
        execv(argv[0], argv);
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
    else if (cmd.action == ERROR){
        // TODO
        // Log error
        std::cerr << cmd.error << std::endl;
        return EXIT_FAILURE;
    }

    if (cmd.organization.length() == 0){
        // TODO
        // Log error
        // use stdlib instead
        std::cout << "Error: Bad input for organization." << std::endl;
        
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

    if (cmd.action == INFO){
        // TODO
    }
    else if (cmd.action == DEPLOY){
        // TODO
        Deployment d;
        char dpath[256];
        sprintf(dpath, "/mnt/dcimages/%s/config/deployment", cmd.vmid.c_str());

        int res = this->parser.ParseDeployment(dpath, &d);

        if (res == EXIT_FAILURE){
            std::cout << "Error: parsing deployment" << std::endl;
            return res;
        }

        // int (*f)(void *) = deploy;
        // this->Sandbox(&d, f);



        char interface[30];
        char bridge[32];
        sprintf(interface, "vxlan%s", (char *)const_cast<char*>(d.vni.c_str()));
        sprintf(bridge, "br-vxlan%s", (char *)const_cast<char*>(d.vni.c_str()));

        //ip link add vxlan1 type vxlan id 1 dev eno1 dstport 0
        std::vector<char *> createlink;
        createlink.push_back((char *)"/usr/sbin/ip");
        createlink.push_back((char *)"link");
        createlink.push_back((char *)"add");
        createlink.push_back((char *)interface);
        createlink.push_back((char *)"type");
        createlink.push_back((char *)"vxlan");
        createlink.push_back((char *)"id");
        createlink.push_back((char *)(char *)const_cast<char*>(d.vni.c_str()));
        createlink.push_back((char *)"dev");
        createlink.push_back((char *)"eno1");
        createlink.push_back((char *)"dstport");
        createlink.push_back((char *)"0");
        createlink.push_back(NULL);

        //bridge fdb append to 00:00:00:00:00:00 dst 192.168.1.101 dev vxlan1
        std::vector<char *> addfdb;
        addfdb.push_back((char *)"/usr/sbin/bridge");
        addfdb.push_back((char *)"fdb");
        addfdb.push_back((char *)"append");
        addfdb.push_back((char *)"to");
        addfdb.push_back((char *)"00:00:00:00:00:00");
        addfdb.push_back((char *)"dst");
        addfdb.push_back((char *)"192.168.1.101");
        addfdb.push_back((char *)"dev");
        addfdb.push_back((char *)interface);
        addfdb.push_back(NULL);

        //ip link add br-vxlan1 type bridge
        std::vector<char *> createbridge;
        createbridge.push_back((char *)"/usr/sbin/ip");
        createbridge.push_back((char *)"link");
        createbridge.push_back((char *)"add");
        createbridge.push_back((char *)bridge);
        createbridge.push_back((char *)"type");
        createbridge.push_back((char *)"bridge");
        createbridge.push_back(NULL);

        //ip link set vxlan1 master br-vxlan1
        std::vector<char *> setmaster;
        setmaster.push_back((char *)"/usr/sbin/ip");
        setmaster.push_back((char *)"link");
        setmaster.push_back((char *)"set");
        setmaster.push_back((char *)interface);
        setmaster.push_back((char *)"master");
        setmaster.push_back((char *)bridge);
        setmaster.push_back(NULL);

        //ip link set vxlan1 up
        std::vector<char *> upinterface;
        upinterface.push_back((char *)"/usr/sbin/ip");
        upinterface.push_back((char *)"link");
        upinterface.push_back((char *)"set");
        upinterface.push_back((char *)interface);
        upinterface.push_back((char *)"up");
        upinterface.push_back(NULL);

        //ip link set br-vxlan1 up
        std::vector<char *> upbridge;
        upbridge.push_back((char *)"/usr/sbin/ip");
        upbridge.push_back((char *)"link");
        upbridge.push_back((char *)"set");
        upbridge.push_back((char *)bridge);
        upbridge.push_back((char *)"up");
        upbridge.push_back(NULL);

        char bootdisk[256];
        char configdisk[256];
        char networkconfig[256];
        sprintf(bootdisk, "path=/mnt/dcimages/%s/disks/boot.qcow2,device=disk", (char *)const_cast<char*>(d.vmid.c_str()));
        sprintf(configdisk, "path=/mnt/dcimages/%s/disks/seed.qcow2,device=disk", (char *)const_cast<char*>(d.vmid.c_str()));
        sprintf(networkconfig, "bridge=br-vxlan%s,model=virtio,mac=%s", (char *)const_cast<char*>(d.vni.c_str()), (char *)const_cast<char*>(d.mac.c_str()));
        
        std::vector<char *> spawnvm;
        spawnvm.push_back((char *)"/usr/bin/virt-install");
        spawnvm.push_back((char *)"--virt-type");
        spawnvm.push_back((char *)"kvm");
        spawnvm.push_back((char *)"--name");
        spawnvm.push_back((char *)const_cast<char*>(d.vmid.c_str()));
        spawnvm.push_back((char *)"--ram");
        spawnvm.push_back((char *)const_cast<char*>(d.ram.c_str()));
        spawnvm.push_back((char *)"--vcpus");
        spawnvm.push_back((char *)const_cast<char*>(d.vcpu.c_str()));
        spawnvm.push_back((char *)"--os-type");
        spawnvm.push_back((char *)const_cast<char*>(d.ostype.c_str()));
        spawnvm.push_back((char *)"--disk");
        spawnvm.push_back((char *)bootdisk);
        spawnvm.push_back((char *)"--disk");
        spawnvm.push_back((char *)configdisk);
        // TODO
        // Attatch additional disks?
        spawnvm.push_back((char *)"--import");
        spawnvm.push_back((char *)"--network");
        spawnvm.push_back((char *)networkconfig);
        spawnvm.push_back((char *)"--noautoconsole");
        spawnvm.push_back(NULL);

        this->Sandbox(&createlink[0]);
        this->Sandbox(&addfdb[0]);
        this->Sandbox(&createbridge[0]);
        this->Sandbox(&setmaster[0]);
        this->Sandbox(&upinterface[0]);
        this->Sandbox(&upbridge[0]);
        this->Sandbox(&spawnvm[0]);
    }
    else if (cmd.action == DESTROY){
        // TODO
    }
    else if (cmd.action == UPDATEFDB){
        // TODO
    }
    return retval;
}