#ifndef MY_STDLIB_H
#define MY_STDLIB_H
#include <stdlib.h>

#define ERROR -1

int init_heap(void);
void* my_malloc(size_t size);
void my_free(void* ptr);
void destroy_heap();
#endif