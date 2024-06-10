#define _GNU_SOURCE
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>

#define ERROR -1
#define STACK_SIZE (1024 * 1024)

void recursive_function(int depth) {
    if(depth == 0)
        return;
    char str[] = "hello world\n";
    int new_depth = --depth;
    // sleep(3);
    recursive_function(new_depth);
}

int child_entrypoint() {
    // printf("Child process startedfasdfnasdfjanfakdsfn!!!\n");
    int init_depth = 10;
    recursive_function(10);

    exit(0);
}

int main(void) {
    int stack_fd = open("../resources/stack.txt", O_RDWR);
    if(stack_fd == -1) {
        perror("fopen error:");
        return ERROR;
    }
    void* stack = mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, stack_fd, 0);
    if(stack == MAP_FAILED) {
        perror("mmap error");
        return ERROR;
    }
    
    pid_t child_pid = clone(child_entrypoint, stack + STACK_SIZE, 0, NULL);

    // printf("Jiv");
    if(child_pid == -1) {
        perror("clone error:");
        return ERROR;
    }
    wait(NULL);
    munmap(stack, STACK_SIZE);
    close(stack_fd);
    

    return 0;
}