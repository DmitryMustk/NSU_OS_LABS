#include "util.h"

void swap(char* a, char* b) {
    const char tmp = *a;
    *a = *b;
    *b = tmp;
}

