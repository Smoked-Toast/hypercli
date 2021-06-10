#include "../include/Controller.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/if.h>
#include <ifaddrs.h>

class Network {
public:
    char * interface;
    char * bridge;
    char * vni;

    Network(char * v){
        vni = new char[8];
        interface = new char[20];
        bridge = new char[22];
        sprintf(interface, "vxlan%s", v);
        sprintf(bridge, "br-vxlan%s", v);
        sprintf(vni, "%s", v);
    }
    ~Network(){
        delete [] interface;
        delete [] bridge;
        delete [] vni;
    }
};

// Function prototypes.
void createlinkBuilder(std::vector<char *> * cmd, Network * n);
void addfdbBuilder(std::vector<char *> * cmd, Network * n);
void createbridgeBuilder(std::vector<char *> * cmd, Network * n);
void setmasterBuilder(std::vector<char *> * cmd, Network * n);
void upinterfaceBuilder(std::vector<char *> * cmd, Network * n);
void upbridgeBuilder(std::vector<char *> * cmd, Network * n);
void spawnvmBuilder(std::vector<char *> * cmd, Deployment * d);

int is_interface_online(std::string interface);


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

        Network n((char *)const_cast<char*>(d.vni.c_str()));
        

        char bootdisk[256];
        char configdisk[256];
        char networkconfig[256];
        sprintf(bootdisk, "path=/mnt/dcimages/%s/disks/boot.qcow2,device=disk", (char *)const_cast<char*>(d.vmid.c_str()));
        sprintf(configdisk, "path=/mnt/dcimages/%s/disks/seed.qcow2,device=disk", (char *)const_cast<char*>(d.vmid.c_str()));
        sprintf(networkconfig, "bridge=br-vxlan%s,model=virtio,mac=%s", (char *)const_cast<char*>(d.vni.c_str()), (char *)const_cast<char*>(d.mac.c_str()));
        d.bootdisk = bootdisk;
        d.configdisk = configdisk;
        d.networkconfig = networkconfig;

        //ip link add vxlan1 type vxlan id 1 dev eno1 dstport 0
        std::vector<char *> createlink;
        createlinkBuilder(&createlink, &n);

        //bridge fdb append to 00:00:00:00:00:00 dst 192.168.1.101 dev vxlan1
        std::vector<char *> addfdb;
        addfdbBuilder(&addfdb, &n);

        //ip link add br-vxlan1 type bridge
        std::vector<char *> createbridge;
        createbridgeBuilder(&createbridge, &n);

        //ip link set vxlan1 master br-vxlan1
        std::vector<char *> setmaster;
        setmasterBuilder(&setmaster, &n);

        //ip link set vxlan1 up
        std::vector<char *> upinterface;
        upinterfaceBuilder(&upinterface, &n);

        //ip link set br-vxlan1 up
        std::vector<char *> upbridge;
        upbridgeBuilder(&upbridge, &n);
        
        std::vector<char *> spawnvm;
        spawnvmBuilder(&spawnvm, &d);

        if (is_interface_online(n.interface) == 0){
            this->Sandbox(&createlink[0]);
            this->Sandbox(&addfdb[0]);
            this->Sandbox(&createbridge[0]);
            this->Sandbox(&setmaster[0]);
            this->Sandbox(&upinterface[0]);
            this->Sandbox(&upbridge[0]);

            printf("started interface");
        }
        this->Sandbox(&spawnvm[0]);
    }
    else if (cmd.action == DESTROY){
        // // TODO
        // Deployment d;
        // char dpath[256];
        // sprintf(dpath, "/mnt/dcimages/%s/config/deployment", cmd.vmid.c_str());
        // int res = this->parser.ParseDeployment(dpath, &d);

        // if (res == EXIT_FAILURE){
        //     std::cout << "Error: parsing deployment" << std::endl;
        //     return res;
        // }

        // Network n((char *)const_cast<char*>(d.vni.c_str()));

        // //virsh shutdown vmid
        // std::vector<char *> shutdownvm;
        // shutdownvmBuilder(&shutdownvm, &d);

        // //virsh undefine vmid

    }
    else if (cmd.action == UPDATEFDB){
        // TODO
    }
    return retval;
}

void createlinkBuilder(std::vector<char *> * cmd, Network * n){
    cmd->push_back((char *)"/usr/sbin/ip");
    cmd->push_back((char *)"link");
    cmd->push_back((char *)"add");
    cmd->push_back((char *)n->interface);
    cmd->push_back((char *)"type");
    cmd->push_back((char *)"vxlan");
    cmd->push_back((char *)"id");
    cmd->push_back(n->vni);
    cmd->push_back((char *)"dev");
    cmd->push_back((char *)"eno1");
    cmd->push_back((char *)"dstport");
    cmd->push_back((char *)"0");
    cmd->push_back(NULL);
}

void addfdbBuilder(std::vector<char *> * cmd, Network * n){
    cmd->push_back((char *)"/usr/sbin/bridge");
    cmd->push_back((char *)"fdb");
    cmd->push_back((char *)"append");
    cmd->push_back((char *)"to");
    cmd->push_back((char *)"00:00:00:00:00:00");
    cmd->push_back((char *)"dst");
    cmd->push_back((char *)"192.168.1.101");
    cmd->push_back((char *)"dev");
    cmd->push_back((char *)n->interface);
    cmd->push_back(NULL);
}

void createbridgeBuilder(std::vector<char *> * cmd, Network * n){
    cmd->push_back((char *)"/usr/sbin/ip");
    cmd->push_back((char *)"link");
    cmd->push_back((char *)"add");
    cmd->push_back((char *)n->bridge);
    cmd->push_back((char *)"type");
    cmd->push_back((char *)"bridge");
    cmd->push_back(NULL);
}

void setmasterBuilder(std::vector<char *> * cmd, Network * n){
    cmd->push_back((char *)"/usr/sbin/ip");
    cmd->push_back((char *)"link");
    cmd->push_back((char *)"set");
    cmd->push_back((char *)n->interface);
    cmd->push_back((char *)"master");
    cmd->push_back((char *)n->bridge);
    cmd->push_back(NULL);
}

void upinterfaceBuilder(std::vector<char *> * cmd, Network * n){
    cmd->push_back((char *)"/usr/sbin/ip");
    cmd->push_back((char *)"link");
    cmd->push_back((char *)"set");
    cmd->push_back((char *)n->interface);
    cmd->push_back((char *)"up");
    cmd->push_back(NULL);
}

void upbridgeBuilder(std::vector<char *> * cmd, Network * n){
    cmd->push_back((char *)"/usr/sbin/ip");
    cmd->push_back((char *)"link");
    cmd->push_back((char *)"set");
    cmd->push_back((char *)n->bridge);
    cmd->push_back((char *)"up");
    cmd->push_back(NULL);
}

void spawnvmBuilder(std::vector<char *> * cmd, Deployment * d){

    cmd->push_back((char *)"/usr/bin/virt-install");
    cmd->push_back((char *)"--virt-type");
    cmd->push_back((char *)"kvm");
    cmd->push_back((char *)"--name");
    cmd->push_back((char *)const_cast<char*>(d->vmid.c_str()));
    cmd->push_back((char *)"--ram");
    cmd->push_back((char *)const_cast<char*>(d->ram.c_str()));
    cmd->push_back((char *)"--vcpus");
    cmd->push_back((char *)const_cast<char*>(d->vcpu.c_str()));
    cmd->push_back((char *)"--os-type");
    cmd->push_back((char *)const_cast<char*>(d->ostype.c_str()));
    cmd->push_back((char *)"--disk");
    cmd->push_back((char *)const_cast<char*>(d->bootdisk.c_str()));
    cmd->push_back((char *)"--disk");
    cmd->push_back((char *)const_cast<char*>(d->configdisk.c_str()));
    // TODO
    // Attatch additional disks?
    cmd->push_back((char *)"--import");
    cmd->push_back((char *)"--network");
    cmd->push_back((char *)const_cast<char*>(d->networkconfig.c_str()));
    cmd->push_back((char *)"--noautoconsole");
    cmd->push_back(NULL);
}

void shutdownvmBuilder(std::vector<char *> * cmd, Deployment * d){
    cmd->push_back((char *)"/usr/bin/virsh");
    cmd->push_back((char *)"shutdown");
    cmd->push_back((char *)const_cast<char*>(d->vmid.c_str()));
    cmd->push_back(NULL);
}

void undefinevmBuilder(std::vector<char *> * cmd, Deployment * d){
    cmd->push_back((char *)"/usr/bin/virsh");
    cmd->push_back((char *)"undefine");
    cmd->push_back((char *)const_cast<char*>(d->vmid.c_str()));
    cmd->push_back(NULL);
}

void deleteinterfaceBuilder(std::vector<char *> * cmd, Network * n){
    cmd->push_back((char *)"/usr/sbin/ip");
    cmd->push_back((char *)"link");
    cmd->push_back((char *)"delete");
    cmd->push_back((char *)n->interface);
    cmd->push_back(NULL);
}

void deletebridgeBuilder(std::vector<char *> * cmd, Network * n){
    cmd->push_back((char *)"/usr/sbin/ip");
    cmd->push_back((char *)"link");
    cmd->push_back((char *)"delete");
    cmd->push_back((char *)n->bridge);
    cmd->push_back(NULL);
}

int is_interface_online(std::string interface) {
    std::vector<char *> inets;
    struct ifaddrs *addresses;
    if (getifaddrs(&addresses) == -1) {
        printf("getifaddrs call failed\n");
        return -1;
    }

    struct ifaddrs *address = addresses;
    while (address) {
        int family = address->ifa_addr->sa_family;
        if (family == AF_INET || family == AF_INET6) {
            inets.push_back(address->ifa_name);
        }
        address = address->ifa_next;
    }
    freeifaddrs(addresses);

    if(std::find(inets.begin(), inets.end(), interface) != inets.end()){
        return 1;
    }
    else {
        return 0;
    }
}