#include "util.h"

#include <stdlib.h>
#include <string.h>

size_t find_func_name_start_index(const char* path_to_dir) {
    size_t last_slash_index = 0;
    for(size_t i = 0; path_to_dir[i] != '\0'; ++i) {
        if(path_to_dir[i] == '/' && path_to_dir[i + 1] != '\0') {
            last_slash_index = i;
        }
    }
    return ++last_slash_index;
}

void get_func_name(char* path_to_link, char* func_name) {
    size_t path_size = strlen(path_to_link);
    size_t start_index = find_func_name_start_index(path_to_link);
    size_t i;
    for(i = start_index; i < path_size; ++i) {
        func_name[i - start_index] = path_to_link[i];
    }
    func_name[++i] = '\0';
}
