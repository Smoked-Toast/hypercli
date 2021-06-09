#include "../include/Parser.hpp"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

RegexTester::RegexTester() : id_regex("^[a-z0-9]*$") {}

bool RegexTester::Testid(std::string id)
{
    return std::regex_match(id, id_regex);
}

Parser::Parser() {}

Command Parser::ParseArgs(int argc, char *argv[])
{
    Command mycmd;

    const char *const short_opts = "a:o:v:h";
    const option long_opts[] = {
        {"action", required_argument, nullptr, 'a'},
        {"organization", required_argument, nullptr, 'o'},
        {"vmid", required_argument, nullptr, 'v'},
        {"help", no_argument, nullptr, 'h'}};
    mycmd.action = ERROR;
    mycmd.error = "Error: Bad input for action.";

    int opt;
    opterr = 0;
    while ((opt = getopt_long(argc, argv, short_opts, long_opts, nullptr)) != -1)
    {
        switch (opt)
        {
        case 'a':
            mycmd.action = mycmd.GetAction(optarg);
            if (mycmd.action == ERROR)
            {
                return mycmd;
            }
            break;
        case 'o':
            if (this->tester.Testid(optarg))
            {
                mycmd.organization = optarg;
            }
            break;
        case 'v':
            if (this->tester.Testid(optarg))
            {
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

int Parser::ParseDeployment(char * file, Deployment * d) {

    std::fstream newfile;
    newfile.open(file, std::ios::in);
    if (newfile.is_open()) {
        std::string line;
        while (getline(newfile, line)) {
            int i = line.find("=");
            if (i >= 1){
                std::string name = line.substr(0, i);
                std::string property = line.substr(i+1);

                int res = d->add(name, property);
                if (res == EXIT_FAILURE){
                    std::cout << "Error: adding attribute: " << name << std::endl;
                    return res;
                }
            }
        }
        newfile.close();
    }
    else {
        // TODO. Handle Error
        std::cout << "Error: opening deployment file: " << file << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}