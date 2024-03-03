#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <sys/syscall.h>
#include <errno.h>

#define ERROR -1
#define SUCCES 0

int main(int argc, char* argv[]) {
    if(argc < 2) {
        printf("Usage: ./%s PROG [ARGS]\n", argv[0]);
        return ERROR;
    }

    pid_t child_pid;
    long orig_rax;
    struct user_regs_struct regs;

    child_pid = fork();
    if (child_pid == ERROR) {
        perror("fork");
        return ERROR;
    }

    if (child_pid == 0) {
        //Parent Process
        if(ptrace(PTRACE_TRACEME, 0, NULL, NULL) == ERROR) {
            perror("ptrace: TRACME");
            return ERROR;
        }
        if(execvp(argv[1], argv + 1)) {
            perror("execl");
            return ERROR;
        }
    } else {
        //CHILD PROCESS
        int status;
        if (wait(&status) == -1) {
                perror("wait");
                exit(EXIT_FAILURE);
        }
        if (!WIFSTOPPED(status)) {
            fprintf(stderr, "Child process don't stop correctly\n");
            return ERROR;
        }

        while (1) {
            ptrace(PTRACE_SYSCALL, child_pid, NULL, NULL);
            if (wait(&status) == -1) {
                perror("wait");
                exit(EXIT_FAILURE);
            }

            if (!WIFSTOPPED(status)) {
                if (WIFEXITED(status))
                    printf("Child process exited\n");
                else if (WIFSIGNALED(status))
                    printf("Child process killed by signal\n");
                else
                    printf("Unknown child process state\n");
                break;
            }

            ptrace(PTRACE_GETREGS, child_pid, NULL, &regs);
            orig_rax = regs.orig_rax;
            if (orig_rax == -1 && errno != 0) {
                perror("ptrace(PTRACE_PEEKUSER)");
                return ERROR;
            }
            printf("System call number: %ld\n", orig_rax);
        }
    }

    return 0;
}
