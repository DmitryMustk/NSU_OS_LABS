#include "util.h"
#include "help.h"
#include "dir_funcs.h"
#include "file_funcs.h"
#include "hard_link_funcs.h"
#include "permission_funcs.h"
#include "sym_link_funcs.h"

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

    if(strcmp(func_name, "create_dir") == 0)
        return create_dir(argv[1]);
    if(strcmp(func_name, "show_dir") == 0)
        return show_dir(argv[1]); 
    if(strcmp(func_name, "rm_dir") == 0)
        return rm_dir(argv[1]); 
    if(strcmp(func_name, "create_file") == 0)
        return create_file(argv[1]);
    if(strcmp(func_name, "show_file") == 0)
        return show_file(argv[1]); 
    if(strcmp(func_name, "rm_file") == 0)
        return rm_file(argv[1]); 
    if(strcmp(func_name, "rm_hard_link") == 0)
        return rm_hard_link(argv[1]); 
    if(strcmp(func_name, "show_file_permissions") == 0)
        return show_file_permissions(argv[1]);
    if(strcmp(func_name, "rm_sym_link") == 0)
        return rm_sym_link(argv[1]);
    if(strcmp(func_name, "show_sym_link") == 0)
        return show_sym_link(argv[1]);
    if(strcmp(func_name, "show_file_by_symlink") == 0)
        return show_file_by_symlink(argv[1]);
    
    if(argc < 3) {
        printf("Wrong number of args. Check -h --help\n");
        return ERROR;
    }
    
    if(strcmp(func_name, "create_hard_link") == 0) 
        return create_hard_link(argv[1], argv[2]);  
    if(strcmp(func_name, "create_sym_link") == 0) 
        return create_sym_link(argv[1], argv[2]);
    if(strcmp(func_name, "change_file_permissons") == 0) 
        return change_file_permissons(argv[1], argv[2]);   

    printf("Wrong link. Please check the -h --help\n");
    return ERROR;
}

//TODO: rewrite help.txt (add all the links and descrs)

int main(int argc, char** argv) {
    if(process_command_args(argc, argv) == ERROR) {
        printf("\nProgram has been crushed\n");
        return ERROR;
    }
    printf("\nProgram has been executed successfuly\n");
    return 0;
}
