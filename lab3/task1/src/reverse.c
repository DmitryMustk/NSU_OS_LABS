#include "reverse.h"
#include "util.h"

#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

int is_path_correct(const char* path_to_dir) {
    DIR* dir = opendir(path_to_dir);
    if(!dir){
        return 0;
    }
    closedir(dir);
    return 1;
}

int find_dir_name_start_index(const char* path_to_dir) {
    int last_slash_index = 0;
    for(size_t i = 0; path_to_dir[i] != '\0'; ++i) {
        if(path_to_dir[i] == '/' && path_to_dir[i + 1] != '\0') {
            last_slash_index = i;
        }
    }
    return last_slash_index;
}

void get_dir_name(const char* path_to_dir, char* dir_name) {
    int start_index = find_dir_name_start_index(path_to_dir);
    strcpy(dir_name, path_to_dir + start_index + 1);
}

char* reverse_name(char* name) {
    size_t name_size = strlen(name);
    char* reversed_name = malloc(sizeof(char) * name_size);
    if(reversed_name == NULL) {
        
    }
    for(size_t i = 0; i < name_size / 2; ++i) {
        reversed_name[i] = name[name_size - i - 1];
        reversed_name[name_size - i - 1] = name[i];
    }
}

int create_reverse_dir(char* dir_name) {
    reverse_name(dir_name);
    mkdir(dir_name, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}