#ifndef PARSER_H
#define PARSER_H
#include <iostream>
#include "Command.hpp"
#include <regex>

class RegexTester {
    public:
        RegexTester();
        bool Testid(std::string id);
    
    private:
        // An id can have lowercase letters and numbers
        std::regex id_regex;
};
/**
 * 
 */
class Parser {
    public:
        Parser();
        Command ParseArgs(int argc, char * argv[]);
    private:
        RegexTester tester;
};


#endif