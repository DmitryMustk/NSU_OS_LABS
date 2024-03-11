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

    #if 0
    if(create_reverse_dir(path_to_dir) != 0) {
        perror("Failed to create reverse dir");
        return ERROR;
    }
    #endif

    char* dir_name = malloc(sizeof(char) * 256);
    get_dir_name(path_to_dir, dir_name);
    // reverse_name(dir_name);
    create_reverse_dir(dir_name);
    printf("|||%s|||\n", dir_name);
    
    // char a = 'a';
    // char b = 'b';
    // swap(&a, &b);
    // char* test = "ol\0";
    // swap(test, test + 1);
    // sw(&test);
    // printf("%s", test);
    // printf("a = %c\nb = %c\n", a, b);
    return 0;
}