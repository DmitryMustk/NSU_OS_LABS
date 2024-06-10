#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ERROR -1

int main() {
    pid_t father_pid = fork();
    if(father_pid == ERROR) {
        perror("fork error");
        return ERROR;
    }

    //father process
    if(father_pid == 0) {
        pid_t child_pid = fork();
        if(child_pid == ERROR) {
            perror("fork error");
            return ERROR;
        }
        //child process
        if(child_pid == 0) {
            while (1) {
                sleep(1);
                printf("I'm a child process\n");
            }
            
        }
        else{
            sleep(6);
            exit(5);
        }
    }
    //granfather process
    else{
        //in grandfather process we don't handle the death of parent process and made him zombie
        while(1) {
            sleep(1);
            printf("I'm a grandfather process");
        }
    }
}