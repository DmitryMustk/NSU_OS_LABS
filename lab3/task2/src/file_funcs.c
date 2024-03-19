#include "file_funcs.h"
#include "hard_link_funcs.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>

int create_file(char* file_path) {
    FILE* file_stream = fopen(file_path, "w");
    if(file_stream == NULL) {
        perror("fcreate error: ");
        return ERROR;
    }
    fclose(file_stream);
    return 0; 
}

int show_file(char* file_path) {
    FILE* file_stream = fopen(file_path, "r");
    if(file_stream == NULL) {
        perror("fcreate error: ");
        return ERROR;
    }
    char buffer[CHAR_BUF_SIZE];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, CHAR_BUF_SIZE, file_stream)) > 0) {
        fwrite(buffer, 1, bytes_read, stdout);
    }
    if(ferror(file_stream)) {
        perror("read_file error: ");
        fclose(file_stream);
        return ERROR;
    }
    fclose(file_stream);
    return 0;
}

int rm_file(char* file_path) {
    return rm_hard_link(file_path);
}
