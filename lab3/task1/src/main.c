#include <dirent.h>
#include <stdio.h>

#define ERROR_CODE -1

int main(int argc, char* argv[]) {
    DIR *dp;
    struct dirent *dirp;
    
    if(argc != 2) {
        printf("Usage: ls dir_name");
	return ERROR_CODE;
    }

    if((dp = opendir(argv[1])) == NULL) {
	printf("Can't open %s", argv[1]);
	return ERROR_CODE;
    }

    while ((dirp = readdir(dp)) != NULL)
	printf("%s\n", dirp->d_name);

    closedir(dp);

    return 0;
}

