#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/syscall.h>

ssize_t write_wrap(int fd, const char* buf, size_t count){
    return syscall(SYS_write, fd, buf, count);
}

int main(){
    const char* str = "Hello world\n";
    ssize_t ret = write_wrap(STDOUT_FILENO, str, strlen(str));
    if(ret == -1){
        return -1;
    }
    return 0;
}