#include "permission_funcs.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>
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

static mode_t mode_from_string(char* mode_str) {
    char* end_ptr = NULL;
    mode_t permissions = strtol(mode_str, &end_ptr, 8);
    if (end_ptr == mode_str || *end_ptr != '\0' || permissions > 0777) {
        printf("Wrong mode\n");
        return ERROR;
    }
    return permissions;
}

int change_file_permissons(char* file_name, char* mode_str) {
    mode_t permissions = mode_from_string(mode_str);
    if(permissions == ERROR) {
        return ERROR;
    }
    int ret = chmod(file_name, permissions);
    if(ret == -1) {
        perror("chmod error: ");
        return ERROR;
    }
    return 0;
}
