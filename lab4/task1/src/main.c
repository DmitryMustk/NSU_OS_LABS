#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 100
#define ERROR -1

int global_init = 1;
int global_ninit;
const int global_const;

void print_var_adresses(void) {
    int local;
    static int stat;
    const int constant; 

    printf("Local var adress: %p\n", &local);
    printf("Local static var adress: %p\n", &stat);
    printf("Local const var adress: %p\n", &constant);
    printf("Global init var adress: %p\n", &global_init);
    printf("Global ninit var adress: %p\n", &global_ninit);
    printf("Global const var adress: %p\n", &global_const);
} 

int heap_manipulations(void) {
    const char* message = "hello world\0";
    size_t msg_len = strlen(message);
    char* buf1 = (char*)malloc(BUF_SIZE * sizeof(char));
    if(buf1 == NULL) {
        perror("malloc error:");
        return ERROR;
    }
    memcpy(buf1, message, msg_len);
    printf("%s\n", buf1);
    free(buf1);
    printf("%s\n", buf1);

    char* buf2 = (char*)malloc(BUF_SIZE * sizeof(char));
    if(buf2 == NULL) {
        perror("malloc error:");
        return ERROR;
    }
    memcpy(buf2, message, msg_len);
    printf("%s\n", buf2);
    free(buf2 + msg_len / 2);
    printf("%s\n", buf2);
    free(buf2);
}


int* get_local_addr(void) {
    int a = 5;
    return &a;
}

int main(void) {
    #ifdef a
	print_var_adresses();
    while(1){
        sleep(1);
    }
    #endif

    #ifdef d
    printf("%p", get_local_addr()); //contains NULL
    #endif
    
    #ifdef e
    int ret = heap_manipulations();
    return ret;
    #endif
    
    return 0;
}

