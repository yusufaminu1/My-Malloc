#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"


int main() {
    int *p = malloc(sizeof(int) * 100);
    int *q = p;
    if (p == NULL) {
        printf("malloc failed unexpectedly\n");
        return EXIT_FAILURE;
    }
    free(p);
    printf("Calling free() a second time on the same pointer...\n");
    free(q);
    /* Should never reach here */
    printf("FAIL: process should have exited with code 2\n");
    return EXIT_SUCCESS;
}
