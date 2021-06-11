#include "../include/ExecBuilder.hpp"
#include "../include/Network.hpp"
#include "../include/Deployment.hpp"

void createlinkBuilder(std::vector<char *> *cmd, Network *n)
{
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

void addfdbBuilder(std::vector<char *> *cmd, Network *n)
{
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

void createbridgeBuilder(std::vector<char *> *cmd, Network *n)
{
    cmd->push_back((char *)"/usr/sbin/ip");
    cmd->push_back((char *)"link");
    cmd->push_back((char *)"add");
    cmd->push_back((char *)n->bridge);
    cmd->push_back((char *)"type");
    cmd->push_back((char *)"bridge");
    cmd->push_back(NULL);
}

void setmasterBuilder(std::vector<char *> *cmd, Network *n)
{
    cmd->push_back((char *)"/usr/sbin/ip");
    cmd->push_back((char *)"link");
    cmd->push_back((char *)"set");
    cmd->push_back((char *)n->interface);
    cmd->push_back((char *)"master");
    cmd->push_back((char *)n->bridge);
    cmd->push_back(NULL);
}

void upinterfaceBuilder(std::vector<char *> *cmd, Network *n)
{
    cmd->push_back((char *)"/usr/sbin/ip");
    cmd->push_back((char *)"link");
    cmd->push_back((char *)"set");
    cmd->push_back((char *)n->interface);
    cmd->push_back((char *)"up");
    cmd->push_back(NULL);
}

void upbridgeBuilder(std::vector<char *> *cmd, Network *n)
{
    cmd->push_back((char *)"/usr/sbin/ip");
    cmd->push_back((char *)"link");
    cmd->push_back((char *)"set");
    cmd->push_back((char *)n->bridge);
    cmd->push_back((char *)"up");
    cmd->push_back(NULL);
}

void spawnvmBuilder(std::vector<char *> *cmd, Deployment *d)
{

    cmd->push_back((char *)"/usr/bin/virt-install");
    cmd->push_back((char *)"--virt-type");
    cmd->push_back((char *)"kvm");
    cmd->push_back((char *)"--name");
    cmd->push_back(d->vmid);
    cmd->push_back((char *)"--ram");
    cmd->push_back(d->ram);
    cmd->push_back((char *)"--vcpus");
    cmd->push_back(d->vcpu);
    cmd->push_back((char *)"--os-type");
    cmd->push_back(d->ostype);
    cmd->push_back((char *)"--disk");
    cmd->push_back(d->bootdisk);
    cmd->push_back((char *)"--disk");
    cmd->push_back(d->configdisk);
    // TODO
    // Attatch additional disks?
    cmd->push_back((char *)"--import");
    cmd->push_back((char *)"--network");
    cmd->push_back(d->networkconfig);
    cmd->push_back((char *)"--noautoconsole");
    cmd->push_back(NULL);
}

void destroyvmBuilder(std::vector<char *> *cmd, Deployment *d)
{
    cmd->push_back((char *)"/usr/bin/virsh");
    cmd->push_back((char *)"destroy");
    cmd->push_back(d->vmid);
    cmd->push_back(NULL);
}

void undefinevmBuilder(std::vector<char *> *cmd, Deployment *d)
{
    cmd->push_back((char *)"/usr/bin/virsh");
    cmd->push_back((char *)"undefine");
    cmd->push_back(d->vmid);
    cmd->push_back(NULL);
}

void deleteinterfaceBuilder(std::vector<char *> *cmd, Network *n)
{
    cmd->push_back((char *)"/usr/sbin/ip");
    cmd->push_back((char *)"link");
    cmd->push_back((char *)"delete");
    cmd->push_back((char *)n->interface);
    cmd->push_back(NULL);
}

void deletebridgeBuilder(std::vector<char *> *cmd, Network *n)
{
    cmd->push_back((char *)"/usr/sbin/ip");
    cmd->push_back((char *)"link");
    cmd->push_back((char *)"delete");
    cmd->push_back((char *)n->bridge);
    cmd->push_back(NULL);
}