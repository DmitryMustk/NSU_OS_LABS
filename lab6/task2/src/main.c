#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#define ERROR -1

int main(void) {
    int pipe_fd[2];
    if (pipe(pipe_fd) == ERROR) {
        perror("pipe error");
        return ERROR;
    }

    pid_t child_pid = fork();
    if(child_pid == ERROR) {
        perror("fork error");
        return ERROR;
    }

    //Child process
    if(child_pid == 0) {
        close(pipe_fd[0]); //unused read end
        uint32_t i = 0;
        while(1) {
            ssize_t ret = write(pipe_fd[1], &i, sizeof(uint32_t));
            if(ret == ERROR) {
                perror("write error");
                return ERROR;
            }
            ++i;
        }
    }
    else {
        close(pipe_fd[1]); //unused write end
        uint32_t prev_val = 0;
        while(1) {
            uint32_t curr_val;
            read(pipe_fd[0], &curr_val, sizeof(uint32_t));
            if(curr_val < prev_val) {
                printf("The sequence is not in increasing order!!! prev: %d; cur: %d\n", prev_val, curr_val);
            }
            prev_val = curr_val;
        }
    }

    return 0;
}

