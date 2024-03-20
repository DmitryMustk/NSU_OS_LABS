#include "permission_funcs.h"
#include "util.h"

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

int show_file_permissions(char* file_name) {
    struct stat file_stat;
    int ret = stat(file_name, &file_stat);
    if(ret == -1) {
        perror("stat error: ");
        return ERROR;
    }
    printf("File permisson bits: %X\n", file_stat.st_mode);
    printf("Number of hard links to file: %ld\n", file_stat.st_nlink);
    return 0;
}

// static mode_t mode_from_string(char* mode_string) {

// }

// int change_file_permissons(char* file_name) {

// }