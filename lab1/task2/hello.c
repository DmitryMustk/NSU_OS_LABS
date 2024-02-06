#include <stdio.h>
#include "hello_static.h"

void print_hello(void){
    printf("Hello world");
}

int main(int argc, char **argv) {
    print_hello();
    print_hello_static();
    return 0;
}
