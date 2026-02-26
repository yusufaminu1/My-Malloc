#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"


int main() {
    char *ptrs[64];
    int i;

    for (i = 0; i < 64; i++) {
        ptrs[i] = malloc(56);
        if (ptrs[i] == NULL) {
            printf("FAIL: malloc returned NULL on chunk %d of 64\n", i);
            return EXIT_FAILURE;
        }
    }

    for (i = 0; i < 64; i++) {
        free(ptrs[i]);
    }
    void *p = malloc(5000);
    if (p != NULL) {
        printf("FAIL: malloc returned non-NULL for 5000 bytes"
               " (heap max payload is 4088 bytes)\n");
        free(p);
        return EXIT_FAILURE;
    }

    printf("PASS test_toolarge_coalesced (malloc correctly returned NULL, error on stderr)\n");
    return EXIT_SUCCESS;
}
