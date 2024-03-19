#include "util.h"
#include "help.h"
#include "dir_funcs.h"
#include "file_funcs.h"

#include <stdio.h>
#include <string.h>

int process_command_args(int argc, char** argv) {
    if(is_need_help(argc, argv)) {
        print_help();
        return 0;
    }
    if(argc < 2) {
        printf("Wrong number of args. Check -h --help\n");
        return ERROR;
    }

    char func_name[FUNC_NAME_SIZE];
    get_func_name(argv[0], func_name);
    // int ret;
    if(strcmp(func_name, "create_dir") == 0)
        return create_dir(argv[1]);
    if(strcmp(func_name, "show_dir") == 0)
        return show_dir(argv[1]); 
    if(strcmp(func_name, "rm_dir") == 0)
        return rm_dir(argv[1]); 
    if(strcmp(func_name, "create_file") == 0)
        return create_file(argv[1]); 
      
}

int main(int argc, char** argv) {
    #if 0
    if(argc != 2) {
        printf("Wrong number of args. Please check the --help page\n");
        return ERROR;
    }
    #endif
    process_command_args(argc, argv);
    
    return 0;
}

