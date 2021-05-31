#include "../include/Parser.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

Parser::Parser() { }

Command Parser::ParseArgs(int argc, char * argv[]){
    Command mycmd;

    const char* const short_opts = "a:o:v:";
    const option long_opts[] = {
            {"action", required_argument, nullptr, 'a'},
            {"organization", no_argument, nullptr, 'o'},
            {"vmid", required_argument, nullptr, 'v'}
    };


    int opt;
    while ((opt = getopt_long(argc, argv, short_opts, long_opts, nullptr)) != -1) {
        switch (opt) {
        case 'a':
            break;
        case 'o':
            break;
        case 'v':
            break;
        }
    }

    return mycmd;
}