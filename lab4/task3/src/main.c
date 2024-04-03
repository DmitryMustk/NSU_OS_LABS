#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define MEMORY_SIZE 1024 * 1024 //Heap size (1Mb)
#define ERROR -1

typedef struct {
    size_t size;
    int is_free;
} block_header;

void* heap_start = NULL;

int init_heap(void) {
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

    if (header->size == 0) {
        return NULL;
    }

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

int main(void) {
    int ret = init_heap();
    if(ret == ERROR) {
        return ERROR;
    }

    char* ptr1 = (char*)my_malloc(32);
    printf("Allocated ptr1 at address: %p\n", ptr1);

    const char* message = "Hello, world!";
    memcpy(ptr1, message, strlen(message));
    printf("%s\n", ptr1);

    my_free(ptr1);
    printf("Freed ptr1\n");

    destroy_heap();

    return 0;
}
