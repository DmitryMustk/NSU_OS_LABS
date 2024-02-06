#include <stdio.h>
#include "hello_dynamic.h"

void print_hello(void){
    printf("hello dynamic world");
}

int main(int argc, char **argv) {
    print_hello();
    print_hello_from_dynamic_lib();
    return 0;
}

