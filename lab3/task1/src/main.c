#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define ERROR -1
#define CHAR_BUF_SIZE 4096

int is_need_help(int argc, char** argv) {
    for(size_t i = 1; i < argc; ++i) {
        if(strcmp(argv[i], "-h") || strcmp(argv[i], "--help")){
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

int main(int argc, char** argv) {
    if(argc < 2) {
        printf("Wrong number of args. Please check the --help page\n");
        return ERROR;
    }

    if(is_need_help(argc, argv)){
        print_help();
        return 0;
    }

    return 0;
}