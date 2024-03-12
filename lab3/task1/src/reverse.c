#include "reverse.h"

#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

#include <stdio.h>
#include <stdlib.h>

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

char* reverse_name(char* name, error_code* error) { //throws malloc_error
    size_t name_size = strlen(name);
    char* reversed_name = malloc(sizeof(char) * (name_size + 1));
    if(reversed_name == NULL) {
        *error = malloc_error;
        return NULL;
    }
    for (size_t i = 0; i < name_size; ++i) {
        reversed_name[i] = name[name_size - i - 1];
    }
    reversed_name[name_size] = '\0';
    return reversed_name;
}

int create_reverse_dir(char* dir_name, error_code* error) { //throws malloc_error
    char* reversed_dir_name = reverse_name(dir_name, error);
    if(*error != no_error) {
        return 0;
    }
    int ret =  mkdir(reversed_dir_name, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    free(reversed_dir_name);
    return ret;
}

char* create_file_path(char* file_name, char* path, error_code* error) { //throws malloc_error
    size_t file_name_size = strlen(file_name);
    size_t path_size = strlen(path);
    char* path_to_file = malloc(path_size + file_name_size + 2);
    if(path_to_file == NULL) {
        *error = malloc_error;
        return path_to_file;
    }
    for(size_t i = 0; i < path_size; ++i) {
        path_to_file[i] = path[i];
    }
    if(path_to_file[path_size - 1] != '/') {
        path_to_file[path_size] = '/';
        path_size++;
    }
    for(size_t i = path_size; i < path_size + file_name_size; ++i) {
        path_to_file[i] = file_name[i - path_size];
    }
    path_to_file[file_name_size + path_size] = '\0';
    return path_to_file;
}

int create_regular_file(char* file_path, error_code* error) { //throws fcreate_error
    FILE* fp = fopen(file_path, "w");
    if(fp == NULL) {
        *error = fcreate_error;
        return ERROR;
    }
    fclose(fp);
    return 0;
} 

int reverse_file_content(char* src_path, char* dst_path, error_code* error) { //throws fwrite_error, fopen_error
    FILE* src_stream = fopen(src_path, "r");
    if(src_stream == NULL) {
        *error = fopen_error;
        return ERROR;
    }
    FILE* dst_stream = fopen(dst_path, "w");
    if(dst_stream == NULL) {
        *error = fopen_error;
        return ERROR;
    }

    char buffer[CHAR_BUF_SIZE];

    size_t bytes_read;
    while ((bytes_read = fread(buffer, sizeof(char), CHAR_BUF_SIZE, src_stream)) > 0) {
        printf("|||%ld|||\n", bytes_read);
        for(size_t i = 0, j = bytes_read - 1; i < j; ++i, --j) {
            char tmp = buffer[i];
            buffer[i] = buffer[j];
            buffer[j] = tmp;
        }
        size_t bytes_written = fwrite(buffer, sizeof(char), bytes_read, dst_stream);
        if(bytes_written != bytes_read) {
            *error = fwrite_error;
            fclose(src_stream);
            fclose(dst_stream);
            return ERROR;
        }
    }
    fclose(src_stream);
    fclose(dst_stream);
    return 0;
}

int reverse_regular_file(char* file_name, char* path, char* reversed_dir_name, error_code* error) { //throws malloc_error, fcreate_error
    char* reversed_file_name = reverse_name(file_name, error);
    if(error != no_error) {
        return ERROR;
    }
    char* path_to_reversed_file = create_file_path(reversed_file_name, path, error);
    char* path_to_orig_file = create_file_path(file_name, reversed_dir_name, error);
    if(error != no_error) {
        goto free;
        return ERROR;
    }
    if(create_regular_file(path_to_reversed_file, error) != 0) {
        goto free;
        return ERROR;
    } 

    free:
        free(reversed_file_name);
        free(path_to_reversed_file);
        free(path_to_orig_file);
}

int reverse_dir_content(char* path_to_dir, char* reversed_dir_name, error_code* error) { //throws opendir_error, malloc_error
                                                                        //close_dir_error
    DIR* d;
    struct dirent* dir;
    d = opendir(path_to_dir);
    if(d == NULL) {
        *error = opendir_error;
        return ERROR;
    }
    while ((dir = readdir(d)) != NULL) {
        if(dir->d_type != DT_REG) {
            continue;
        }
        // int ret = reverse_regular_file(dir->d_name, error);
    }
    int ret = closedir(d);
    if(ret == -1) {
        *error = closedir_error;
    }
    return ret;
}