#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#define ERROR -1
// #define MEMORY_SIZE 1024 * 1024

int main(void) {
    uint64_t page_size = sysconf(_SC_PAGE_SIZE);
    uint32_t* memory = (uint32_t*) mmap(NULL, page_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    memset(memory, 0, page_size);
    pid_t child_pid = fork();
    if (child_pid == ERROR) {
        perror("fork error:");
        return ERROR;
    }

    //!Parent Process
    if(child_pid == 0) {
        uint32_t num = 0;
        while (1) {
            for(int i = 0; i < page_size / sizeof(uint32_t); ++i) {
                // printf("%d\n", num);
                memory[i] = num++;
            }
        }
        
    }
    //!Child Process
    if(child_pid > 0){
        while (1) {
            sleep(1);
            uint32_t prev = memory[0];
            for(int i = 1; i < page_size / sizeof(uint32_t); ++i) {
                if(prev >= memory[i]) {
                    printf("The sequence is not in increasing order!!! prev: %d; cur: %d\n", prev, memory[i]);
                }
                prev = memory[i];
            }
        }
        
    }
    return 0;
}

