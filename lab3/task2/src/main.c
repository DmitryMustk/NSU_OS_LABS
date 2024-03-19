#include "util.h"
#include "help.h"

#include <stdio.h>

int main(int argc, char** argv) {
    if(argc != 2) {
        printf("Wrong number of args. Please check the --help page\n");
        return ERROR;
    }
    if(is_need_help(argc, argv)) {
        print_help();
        return 0;
    }
    return 0;
}

