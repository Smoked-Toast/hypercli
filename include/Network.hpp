#ifndef NETWORK_H
#define NETWORK_H

int is_interface_online(std::string interface);

class Network {
public:
    char *interface;
    char *bridge;
    char *vni;

    Network(char *v);
    ~Network();
};

#endif