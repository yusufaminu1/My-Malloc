#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"


int main() {
    char *ptrs64[64];
    char *ptrs32[32];
    int i;

    for (i = 0; i < 64; i++) {
        ptrs64[i] = malloc(56);
        if (ptrs64[i] == NULL) {
            printf("FAIL: malloc returned NULL on chunk %d of 64\n", i);
            return EXIT_FAILURE;
        }
    }

    for (i = 0; i < 64; i++) {
        free(ptrs64[i]);
    }

    for (i = 0; i < 32; i++) {
        ptrs32[i] = malloc(120);
        if (ptrs32[i] == NULL) {
            printf("FAIL: malloc returned NULL on chunk %d of 32"
                   " (coalescing after 64-chunk free failed)\n", i);
            return EXIT_FAILURE;
        }
    }

    for (i = 0; i < 32; i++) {
        free(ptrs32[i]);
    }

    printf("PASS test_chunk_refill\n");
    return EXIT_SUCCESS;
}
