#include "lib/hello_runtime_dynamic.h"

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

#define ERROR -1

void (*func_ptr)(void);

int main(void) {
    void* handle = dlopen("../src/lib/libhello_runtime_dynamic.so", RTLD_LAZY);
    if(handle == NULL) {
        fprintf(stderr, "dlopen error\n");
        return ERROR;
    }
    func_ptr = dlsym(handle, "hello_from_dyn_runtime_lib");
    if(func_ptr == NULL) {
        dlclose(handle);
        fprintf(stderr, "dlsym error\n");
        return ERROR;
    }
    func_ptr();
    dlclose(handle);
    return 0;
}
