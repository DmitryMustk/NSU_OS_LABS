#include "reverse.h"
#include "help.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>

void sw(char** a) {
    *(*a) = 'k';
}

int main(int argc, char** argv) {
    if(argc != 2) {
        printf("Wrong number of args. Please check the --help page\n");
        return ERROR;
    }

    if(is_need_help(argc, argv)){
        print_help();
        return 0;
    }

    const char* path_to_dir = argv[1];

    if(!is_path_correct(path_to_dir)){
        printf("Incorrect path. Please check the --help page\n");
        return ERROR;
    }

    error_code error = no_error;
    char* dir_name = malloc(sizeof(char) * 256);
    get_dir_name(path_to_dir, dir_name);
    if(create_reverse_dir(dir_name, error) != 0) {
        perror("Can't create a directory");
        return ERROR;
    }
    if(error == malloc_error) {
        perror("Can't allocate memmory for name string");
        return ERROR;
    } 
    free(dir_name);
    return 0;
}