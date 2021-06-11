#ifndef NETWORK_H
#define NETWORK_H

#include <stdio.h>

class Network {
public:
    char *interface;
    char *bridge;
    char *vni;

    Network(char *v)
    {
        vni = new char[8];
        interface = new char[20];
        bridge = new char[22];
        sprintf(interface, "vxlan%s", v);
        sprintf(bridge, "br-vxlan%s", v);
        sprintf(vni, "%s", v);
    }
    ~Network()
    {
        delete[] interface;
        delete[] bridge;
        delete[] vni;
    }
};

#endif