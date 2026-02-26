#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"


int main() {
    int i;
    for (i = 0; i < 10; i++) {
        malloc(8);
    }
    printf("Exiting without freeing 10 objects (80 bytes).\n");
    printf("Leak detector should report leaks on stderr.\n");
    return EXIT_SUCCESS;
}
