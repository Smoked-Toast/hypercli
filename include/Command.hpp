#ifndef COMMAND_H
#define COMMAND_H
#include <string>

enum Action {
    USAGE=0,
    INFO=1,
    DEPLOY=2,
    DESTROY=3,
    UPDATEFDB=4,


    // Action Error,
    ERROR=-1
};

struct Command {
    std::string organization;
    std::string vmid;
    Action action;
    std::string error;

    Action GetAction(std::string action){
        if (action.compare("info")==0){
            return INFO;
        }
        else if (action.compare("deploy")==0){
            return DEPLOY;
        }
        else if (action.compare("destroy")==0){
            return DESTROY;
        } 
        else if (action.compare("updatefdb")==0){
            return UPDATEFDB;
        }
        else {
            error = "Error: Bad input for action.";
            return ERROR;
        }
    }
};


#endif