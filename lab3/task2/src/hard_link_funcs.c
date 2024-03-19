#include "hard_link_funcs.h"
#include "util.h"

#include <stdio.h>
#include <unistd.h>

int rm_hard_link(char* path_to_hard_link) {
    int ret = unlink(path_to_hard_link);
    if(ret == -1) {
        perror("unlink error: ");
        return ERROR;
    }
    return 0;
}

int create_hard_link(char* path_to_file, char* link_name) {
    int ret = link(path_to_file, link_name);
    if(ret == -1) {
        perror("link error: ");
        return ERROR;
    }
    return 0;
}
