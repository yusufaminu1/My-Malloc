#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"


int main() {
    int x;
    printf("Calling free() with a stack address (not from malloc)...\n");
    free(&x);
    printf("FAIL: process should have exited with code 2\n");
    return EXIT_SUCCESS;
}
