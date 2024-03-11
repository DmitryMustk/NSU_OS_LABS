#ifndef TASK1_UTIL_H
#define TASK1_UTIL_H

#define CHAR_BUF_SIZE 4096
#define ERROR -1

typedef enum error_code {
    no_error = 0,
    malloc_error,
} error_code;

void swap(char* a, char* b);
#endif