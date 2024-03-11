#include "help.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int is_need_help(int argc, char** argv) {
    for(size_t i = 1; i < argc; ++i) {
        if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0){
            return 1;
        }
    }
    return 0;
}

void print_help(void) {
    FILE* help_fp = fopen("../resourses/help.txt", "rb");
    if(help_fp == NULL) {
        perror("Can't open the help.txt file");
        return;
    }

    char* buf = malloc(sizeof(char) * CHAR_BUF_SIZE);
    if(buf == NULL) {
        fclose(help_fp);
        perror("Can't allocate the memory for char buffer");
        return;
    }

    size_t bytes_read;
    while ((bytes_read = fread(buf, 1, CHAR_BUF_SIZE, help_fp)) > 0) {
        if(ferror(help_fp)) {
            free(buf);
            fclose(help_fp);
            perror("Error while reading help.txt file");
            return;
        }

        fwrite(buf, 1, CHAR_BUF_SIZE, stdout);
    }
    free(buf);
    fclose(help_fp);
}