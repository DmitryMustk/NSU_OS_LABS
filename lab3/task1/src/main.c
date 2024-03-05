#include "help.h"
#include "util.h"

#include <stdio.h>
// #include <string.h>
// #include <stdlib.h>
// #include <unistd.h>

int main(int argc, char** argv) {
    if(argc < 2) {
        printf("Wrong number of args. Please check the --help page\n");
        return ERROR;
    }

    if(is_need_help(argc, argv)){
        print_help();
        return 0;
    }

    const char* path_to_dir = argv[1];

    return 0;
}