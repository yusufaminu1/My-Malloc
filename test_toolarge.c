#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

int main() {
    void *p = malloc(5000);
    if (p != NULL) {
        printf("FAIL: malloc returned non-NULL for a 5000-byte request"
               " (heap is only 4096 bytes)\n");
        free(p);
        return EXIT_FAILURE;
    }
    printf("PASS test_toolarge (malloc correctly returned NULL, error on stderr)\n");
    return EXIT_SUCCESS;
}
