#include "file_funcs.h"
#include "hard_link_funcs.h"
#include "sym_link_funcs.h"
#include "util.h"

#include <stdio.h>
#include <unistd.h>

int create_sym_link(char* path_to_file, char* link_name) {
    int ret = symlink(path_to_file, link_name);
    if(ret == -1) {
        perror("symlink error: ");
        return ERROR;
    }
    return 0;
}

int rm_sym_link(char* path_to_link) {
    return rm_hard_link(path_to_link);
}

int show_sym_link(char* path_to_link) {
    char buf[CHAR_BUF_SIZE];
    ssize_t ret = readlink(path_to_link, buf, CHAR_BUF_SIZE);
    if(ret == -1) {
        perror("readlink error: ");
        return ERROR;
    }
    printf("%s\n", buf);
    return 0;
}

int show_file_by_symlink(char* path_to_link) {
    char file_name[CHAR_BUF_SIZE];
    ssize_t ret = readlink(path_to_link, file_name, CHAR_BUF_SIZE);
    if(ret == -1) {
        perror("readlink error: ");
        return ERROR;
    }
    return show_file(file_name);
}