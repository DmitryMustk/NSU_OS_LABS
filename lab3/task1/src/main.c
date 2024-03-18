#include "reverse.h"
#include "help.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    if(argc != 2) {
        printf("Wrong number of args. Please check the --help page\n");
        return ERROR;
    }

    if(is_need_help(argc, argv)){
        print_help();
        return 0;
    }

    char* path_to_dir = argv[1];

    if(!is_path_correct(path_to_dir)){
        printf("Incorrect path. Please check the --help page\n");
        return ERROR;
    }

    error_code error = no_error;
    char* dir_name = malloc(sizeof(char) * 256);
    get_dir_name(path_to_dir, dir_name);
    char* reversed_dir_name = create_reverse_dir(dir_name, &error);
    if(error != no_error) {
        perror("Can't create a directory");
        free(dir_name);
        free(reversed_dir_name);
        return ERROR;
    }
    if(error == malloc_error) {
        perror("Can't allocate memmory for name string");
        free(dir_name);
        free(reversed_dir_name);
        return ERROR;
    }
    reverse_dir_content(path_to_dir, reversed_dir_name, &error);
    if(error != no_error){
        perror("Error");
        free(dir_name);
        free(reversed_dir_name);
        return ERROR;
    }

    free(dir_name);
    free(reversed_dir_name);
    return 0;
}