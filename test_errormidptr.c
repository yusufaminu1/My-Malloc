#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"


int main() {
    int *p = malloc(sizeof(int) * 2);
    if (p == NULL) {
        printf("malloc failed unexpectedly\n");
        return EXIT_FAILURE;
    }
    printf("Calling free() with a pointer one int past the start of a chunk...\n");
    free(p + 1);
    
    printf("FAIL: process should have exited with code 2\n");
    return EXIT_SUCCESS;
}
