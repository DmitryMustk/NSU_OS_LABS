#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

#define ERROR -1
#define PAGE_SIZE 4096

int i = 1;

void recursive_stack_alloc() {
    char arr[4096];
    printf("%d; %d\n", i++, getpid());
    usleep(10000);
    recursive_stack_alloc();
}

void sigsegv_handler(int signum) {
    printf("POIMALI SIGSEGV!!!!!!\n");
    exit(signum);
}

int main(int argc, char** argv) {
    pid_t pid = getpid();
    printf("%d\n", pid);
    #ifdef a
    sleep(10);
    int ret = execv("./task2", argv);
    if(ret == -1) {
        perror("exec error: ");
        return ERROR;
    }
    printf("Hello, world");
    #endif

    #ifdef c
    sleep(10);
    recursive_stack_alloc();
    #endif

    #ifdef cv
    size_t size = 0;
    const int block = 1024 * 10; //brk
    // const int block = 1024 * 1024; //mmap
    printf("pid %d; size %ld\n", getpid(), size);
    sleep(10);

    while(1) {
        char* buf = malloc(block);
        if(buf == NULL) {
            perror("malloc error:");
            return ERROR;
        }
        size += block;
        printf("pid %d; size %ld\n", getpid(), size);
        usleep(100000);
    }
    #endif

    #ifdef c8
    struct sigaction sa;
    sa.sa_handler = sigsegv_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if(sigaction(SIGSEGV, &sa, NULL) == ERROR) {
        perror("sigaction");
        return ERROR;
    }

    char* addr_region = mmap(NULL, 10 * PAGE_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if(addr_region == MAP_FAILED) {
        perror("mmap error");
        return ERROR;
    }
    printf("Created address region: %p\n", &addr_region);
    sleep(10);

    //read
//     int ret = mprotect(addr_region, 10 * PAGE_SIZE, PROT_READ);
//     if (ret == ERROR) {
//         perror("mprotect error:");
//         return ERROR;
//     }
//     printf("Changed mem protection: PROT_READ\n");
//     // *addr_region = '1';
//     sleep(10);

//     //write
//     ret = mprotect(addr_region, 10 * PAGE_SIZE, PROT_WRITE);
//     if (ret == ERROR) {
//         perror("mprotect");
//         exit(EXIT_FAILURE);
//     }
//     printf("Changed memory protection: PROT_WRITE\n");
// //    printf("%c\n", *addr_region);
//     sleep(10);

    int ret = munmap(addr_region + 3 * PAGE_SIZE, 2 * PAGE_SIZE);
    if (ret == ERROR) {
        perror("munmap");
        exit(EXIT_FAILURE);
    }
    printf("Deallocated address region\n");
    sleep(10);
    #endif

    return 0;
}


