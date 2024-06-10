#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define ERROR -1

int global_var;

int main(void) {
    int local_var = 1;
    global_var = 1;

    printf(" Local var addr: %p and value: %d\n", &local_var, local_var);
    printf("Global var addr: %p and value: %d\n", &global_var, global_var);
    pid_t parent_pid = getpid();

    printf("My PID: %d\n", parent_pid);
    
    pid_t child_pid = fork();
    if (child_pid == ERROR) {
        perror("fork error:");
        return ERROR;
    }

    //Child Process
    if(child_pid == 0) {
        printf("=====================CHILD PROCESS=====================\n");
        printf("My (child) pid: %d\n", getpid());
        printf("Parrent pid: %d\n", getppid());
        printf("          Local var addr: %p and value: %d\n", &local_var, local_var);
        printf("         Global var addr: %p and value: %d\n", &global_var, global_var);
        local_var++;
        global_var++;
        printf("Modified  Local var addr: %p and value: %d\n", &local_var, local_var);
        printf("Modified Global var addr: %p and value: %d\n", &global_var, global_var);
        printf("=======================================================\n");
        exit(5);
    }
    //Parent Process
    else {
        sleep(1);
        printf("=====================PARENT PROCESS====================\n");
        printf("          Local var addr: %p and value: %d\n", &local_var, local_var);
        printf("         Global var addr: %p and value: %d\n", &global_var, global_var);
        sleep(30);
        // printf("          Local var addr: %p and value: %d\n", &local_var, local_var);
        // printf("         Global var addr: %p and value: %d\n", &global_var, global_var);
        
        
        int status;
        int terminated_pid = wait(&status);
        if(terminated_pid == -1) {
            perror("wait error:");
            return ERROR;
        }

        //process ended with exit
        if(WIFEXITED(status)) {
            printf("Child process exited with status code: %d\n", WEXITSTATUS(status));
        }
        else if(WIFSIGNALED(status)) {
            printf("Child process terminated with unhandled signal with sig number: %d\n", WTERMSIG(status));
        }
        else if(WIFSTOPPED(status)) {
            printf("Child process been stoped by a signal with sig number: %d\n", WSTOPSIG(status));
        }
        
        printf("=======================================================\n");

    }

    return 0;
}

