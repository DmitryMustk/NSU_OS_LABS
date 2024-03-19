#include "reverse.h"
#include "util.h"

#include <stdio.h>

int main(int argc, char** argv) {
    if(process_command_args(argc, argv) == ERROR) {
        perror("Error: ");
        return ERROR;
    }
    return 0;
}
