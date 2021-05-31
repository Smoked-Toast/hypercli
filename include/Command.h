#ifndef COMMAND_H
#define COMMAND_H

#include <string>
enum Action {
    INFO=0,
    DEPLOY=1,
    DESTROY=2,
    ADDFDB=3,


    //ERRORS are all negative
    ERROR_ARGS=-1
};

struct Command {
    std::string organization;
    std::string vmid;
    Action action;
};


#endif