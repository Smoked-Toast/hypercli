#include "../include/Parser.hpp"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

RegexTester::RegexTester() : id_regex("^[a-z0-9]*$"){ }

bool RegexTester::Testid(std::string id) {
    return std::regex_match(id, id_regex);
}

Parser::Parser() { }

Command Parser::ParseArgs(int argc, char * argv[]){
    Command mycmd;

    const char* const short_opts = "a:o:v:h";
    const option long_opts[] = {
            {"action", required_argument, nullptr, 'a'},
            {"organization", required_argument, nullptr, 'o'},
            {"vmid", required_argument, nullptr, 'v'},
            {"help", no_argument, nullptr, 'h'}
    };
    mycmd.action=ERROR;
    mycmd.error="Error: Bad input for action.";

    int opt;
    opterr = 0;
    while ((opt = getopt_long(argc, argv, short_opts, long_opts, nullptr)) != -1) {
        switch (opt) {
        case 'a':
            mycmd.action = mycmd.GetAction(optarg);
            if (mycmd.action == ERROR){
                return mycmd;
            }
            break;
        case 'o':
            if (this->tester.Testid(optarg)){
                mycmd.organization = optarg;
            }
            break;
        case 'v':
            if (this->tester.Testid(optarg)){
                mycmd.vmid = optarg;
            }
            break;
        case 'h':
            mycmd.action = USAGE;
            return mycmd;
        }
    }

    return mycmd;
}