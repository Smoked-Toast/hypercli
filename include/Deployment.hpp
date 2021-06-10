#ifndef DEPLOYMENT_H
#define DEPLOYMENT_H
#include <stdio.h>
#include <string.h>

class Deployment {
public:
    std::string vmid;
    std::string ram;
    std::string vcpu;
    std::string ostype;
    std::string vni;
    std::string mac;
    std::string bootdisk;
    std::string configdisk;
    std::string networkconfig;

    int add(std::string name, std::string property){
        if (name == "vmid"){
            vmid = property;
        }
        else if (name == "ram"){
            ram = property;
        }
        else if (name == "vcpu"){
            vcpu = property;
        }
        else if (name == "ostype"){
            ostype = property;
        }
        else if (name == "vni"){
            vni = property;
        }
        else if (name == "mac"){
            mac = property;
        } 
        else {
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    }
};

#endif