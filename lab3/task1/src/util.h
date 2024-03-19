#ifndef TASK1_UTIL_H
#define TASK1_UTIL_H

#define CHAR_BUF_SIZE 1
#define ERROR -1

typedef enum error_code {
    no_error = 0,
    malloc_error,
    opendir_error,
    closedir_error,
    fcreate_error,
    fopen_error,
    fwrite_error,
    mkdir_error,
    ftell_error,
    fseek_error,
    fread_error,
} error_code;
#endif
