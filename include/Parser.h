#ifndef PARSER_H
#define PARSER_H
#include <iostream>
#include "Command.h"

class Parser {
    public:
        Parser();
        Command ParseArgs(int argc, char * argv[]);
};


#endif