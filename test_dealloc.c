#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"


static int test_free_deallocates() {
    char *ptrs[200];
    int i;

    for (i = 0; i < 200; i++) {
        ptrs[i] = malloc(1);
        if (ptrs[i] == NULL) {
            printf("FAIL test_free_deallocates: malloc returned NULL on allocation %d\n", i);
            return 1;
        }
    }
    for (i = 0; i < 200; i++) {
        free(ptrs[i]);
    }

    for (i = 0; i < 200; i++) {
        ptrs[i] = malloc(1);
        if (ptrs[i] == NULL) {
            printf("FAIL test_free_deallocates: malloc returned NULL on re-allocation %d"
                   " (free did not deallocate)\n", i);
            return 1;
        }
    }
    for (i = 0; i < 200; i++) {
        free(ptrs[i]);
    }
    printf("PASS test_free_deallocates\n");
    return 0;
}


static int test_coalescing() {
    char *ptrs[200];
    int i;

    for (i = 0; i < 200; i++) {
        ptrs[i] = malloc(1);
        if (ptrs[i] == NULL) {
            printf("FAIL test_coalescing: malloc returned NULL on allocation %d\n", i);
            return 1;
        }
    }
    for (i = 0; i < 200; i++) {
        free(ptrs[i]);
    }

    char *large = malloc(2000);
    if (large == NULL) {
        printf("FAIL test_coalescing: large allocation failed"
               " (adjacent free chunks were not coalesced)\n");
        return 1;
    }
    free(large);
    printf("PASS test_coalescing\n");
    return 0;
}

int main() {
    int failures = 0;
    failures += test_free_deallocates();
    failures += test_coalescing();
    return failures > 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}
