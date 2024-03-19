#include "dir_funcs.h"
#include "util.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int create_dir(char* dir_path) {
    int ret = mkdir(dir_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if(ret != 0) {
        perror("mkdir error:");
        return ERROR;
    }
    return 0;
}

int show_dir(char* dir_path) {
    struct dirent** file_list;
    ssize_t ret = scandir(dir_path, &file_list, NULL, alphasort);
    if(ret == -1) {
        perror("scandir error: ");
        return ERROR;
    }
    for(ssize_t i = 0; i < ret; ++i) {
        printf("%s\n", file_list[i]->d_name);
        free(file_list[i]);
    }
    free(file_list);
    return 0;
}   

int rm_dir(char* dir_path) {
    int ret = rmdir(dir_path);
    if(ret == -1) {
        perror("rmdir error: ");
        return ERROR;
    }
    return 0;
}