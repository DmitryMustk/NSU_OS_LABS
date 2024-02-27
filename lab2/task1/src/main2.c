#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

int main(){
    const char* str = "Hello world\n";
    ssize_t ret = write(STDOUT_FILENO, str, strlen(str));
    if(ret == -1){
        return -1;
    }
    return 0;
}