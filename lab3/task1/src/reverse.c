#include "reverse.h"
#include "help.h"

#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int process_command_args(int argc, char **argv) {
    if(argc != 2) {
        printf("Wrong number of args. Please check the --help page\n");
        return ERROR;
    }

    if(is_need_help(argc, argv)) {
        print_help();
        return 0;
    }

    char* path_to_dir = argv[1];
    if(!is_path_correct(path_to_dir)) {
        printf("Incorrect path. Please check the --help page\n");
        return ERROR;
    }
    error_code error = no_error;
    char* dir_name = malloc(sizeof(char) * 256);
    get_dir_name(path_to_dir, dir_name);
    char* reversed_dir_name = create_reverse_dir(dir_name, &error);
    if(error != no_error) {
        perror("Can't create a directory");
        free(dir_name);
        free(reversed_dir_name);
        return ERROR;
    }
    if(error == malloc_error) {
        perror("Can't allocate memmory for name string");
        free(dir_name);
        free(reversed_dir_name);
        return ERROR;
    }
    reverse_dir_content(path_to_dir, reversed_dir_name, &error);
    if(error != no_error){
        perror("Error");
        free(dir_name);
        free(reversed_dir_name);
        return ERROR;
    }

    free(dir_name);
    free(reversed_dir_name);
    return 0;
}

int is_path_correct(const char* path_to_dir) {
    DIR* dir = opendir(path_to_dir);
    if(dir == NULL){
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

char* create_reverse_dir(char* dir_name, error_code* error) { //throws malloc_error
    char* reversed_dir_name = reverse_name(dir_name, error);
    if(*error != no_error) {
        return 0;
    }
    int ret =  mkdir(reversed_dir_name, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if(ret != 0) {
        *error = mkdir_error;
    }
    return reversed_dir_name;
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

uint64_t get_file_size(const char* file_name, error_code* error) {
    FILE* file_stream = fopen(file_name, "r");
    if(file_stream == NULL) {
        *error = fopen_error;
        return ERROR;
    }
    uint64_t begin_pos = ftell(file_stream);
    if(begin_pos == -1) {
        *error = ftell_error;
        fclose(file_stream);
        return ERROR;
    }
    uint64_t size = 0;

    int ret = fseek(file_stream, 0, SEEK_END);
    if (ret == -1) {
        *error = fseek_error;
        fclose(file_stream);
        return ERROR;
    }

    size = ftell(file_stream);
    if(size == -1) {
        *error = ftell_error;
        fclose(file_stream);
        return ERROR;
    }
    fclose(file_stream);
    return size;
}

void reverse_string(char* string, uint64_t size) {
    for(uint64_t i = 0, j = size - 1; i < j; ++i, --j) {
        const char tmp = string[i];
        string[i] = string[j];
        string[j] = tmp;
    }
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
    uint64_t src_size = get_file_size(src_path, error);
    if(src_size == -1) {
        fclose(src_stream);
        fclose(dst_stream);
        return ERROR;
    }

    char* buffer = malloc(sizeof(char) * src_size);
    if(buffer == NULL) {
        *error = malloc_error;
        fclose(src_stream);
        fclose(dst_stream);
        return ERROR;
    }
    uint64_t bytes_read = fread(buffer, 1, src_size, src_stream);
    if (bytes_read != src_size) {
        *error = fread_error;
        fclose(src_stream);
        fclose(dst_stream);
        free(buffer);
        return ERROR;
    }
    reverse_string(buffer, src_size);

    size_t bytes_written_total = 0;
    while (bytes_written_total < src_size) {
        size_t bytes_to_write = src_size - bytes_written_total;
        if (bytes_to_write > CHAR_BUF_SIZE) {
            bytes_to_write = CHAR_BUF_SIZE;
        }
        size_t bytes_written = fwrite(buffer + bytes_written_total, 1, bytes_to_write, dst_stream);
        if (bytes_written != bytes_to_write) {
            *error = fwrite_error;
            free(buffer);
            fclose(src_stream);
            fclose(dst_stream);
            return ERROR;
        }
        bytes_written_total += bytes_written;
    }

    fclose(src_stream);
    fclose(dst_stream);
    free(buffer);
    return 0;
}

int reverse_regular_file(char* file_name, char* path, char* reversed_dir_name, error_code* error) { //throws malloc_error, fcreate_error
    char* reversed_file_name = reverse_name(file_name, error);
    if(*error != no_error) {
        return ERROR;
    }
    char* path_to_reversed_file = create_file_path(reversed_file_name, reversed_dir_name, error);
    char* path_to_orig_file = create_file_path(file_name, path, error);
    if(*error != no_error) {
        free(reversed_file_name);
        free(path_to_reversed_file);
        free(path_to_orig_file);
        return ERROR;
    }
    if(create_regular_file(path_to_reversed_file, error) != 0) {
        free(reversed_file_name);
        free(path_to_reversed_file);
        free(path_to_orig_file);
        return ERROR;
    }
    int ret = reverse_file_content(path_to_orig_file, path_to_reversed_file, error);
    free(reversed_file_name);
    free(path_to_reversed_file);
    free(path_to_orig_file);
    return ret;     
}

int reverse_dir_content(char* path_to_dir, char* reversed_dir_name, error_code* error) { //throws opendir_error, malloc_error close_dir_error
    DIR* d;
    struct dirent* dir;
    d = opendir(path_to_dir);
    if(d == NULL) {
        *error = opendir_error;
        return ERROR;
    }
    while ((dir = readdir(d)) != NULL) {
        if(dir->d_type != DT_REG) 
            continue;
        int ret = reverse_regular_file(dir->d_name, path_to_dir, reversed_dir_name, error);
        if(ret != 0)
            break;
    }
    int ret = closedir(d);
    if(ret == -1) 
        *error = closedir_error;
    return ret;
}
