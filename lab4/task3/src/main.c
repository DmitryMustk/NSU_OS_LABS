#include "lib/my_stdlib.h"

#include <string.h>
#include <stdio.h>

#define BUF_SIZE 32

int main(void) {
    int ret = init_heap();
    if(ret == ERROR) {
        return ERROR;
    }

    char* ptr1 = (char*)my_malloc(BUF_SIZE * sizeof(char));
    if(ptr1 == NULL) {
        printf("my_malloc error ( 0 _ 0 )\n");
        return ERROR;
    }
    printf("Allocated ptr1 at address: %p\n", ptr1);

    const char* message = "Hello, world!";
    memcpy(ptr1, message, strlen(message));
    printf("%s\n", ptr1);

    my_free(ptr1);
    printf("Freed ptr1\n");
    
    destroy_heap();

    return 0;
}
