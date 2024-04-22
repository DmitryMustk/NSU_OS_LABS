#include "my_stdlib.h"

#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

typedef struct {
    size_t size;
    int is_free;
} block_header;

size_t MEMORY_SIZE = 0;
void* heap_start = NULL;

int init_heap(void) {
    MEMORY_SIZE = sysconf(_SC_PAGESIZE);
    heap_start = mmap(NULL, MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if(heap_start == MAP_FAILED) {
        perror("mmap error:");
        return ERROR;
    }
 
    block_header* header = (block_header*) heap_start;
    header->size = MEMORY_SIZE - sizeof(block_header);
    header->is_free = 1;
}

void destroy_heap() {
    munmap(heap_start, MEMORY_SIZE);
}

void* my_malloc(size_t size) {
    block_header* header = (block_header*) heap_start;

    //Find block
    while (header->size != 0 && (header->is_free == 0 || header->size < size + sizeof(block_header))) {
        header = (block_header*) ((char*) header + sizeof(block_header) + header->size);
    }

    if (header->size == 0) 
        return NULL;
    
    size_t total_size = size + sizeof(block_header);
    if (header->size > total_size + sizeof(block_header)) {
        block_header* new_header = (block_header*) ((char*) header + sizeof(block_header) + total_size);
        new_header->size = header->size - total_size - sizeof(block_header);
        new_header->is_free = 1;
        header->size = total_size;
    }
    header->is_free = 0; 
    return (void*) ((char*) header + sizeof(block_header));
}

void my_free(void* ptr) {
    if(ptr == NULL) {
        return;
    }
    block_header* header = (block_header*) ((char*) ptr - sizeof(block_header));
    header->is_free = 1;
}
