#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

int main() {
    char *ptrs[120] = {NULL};
    int allocations = 0;
    int i;

    srand(12345); 

    while (allocations < 120) {
        int choice = rand() % 2;
        if (choice == 0) {
            char *p = malloc(1);
            if (p != NULL) {
                ptrs[allocations++] = p;
            }
        } else if (allocations > 0) {
            int idx = rand() % allocations;
            if (ptrs[idx] != NULL) {
                free(ptrs[idx]);
                ptrs[idx] = NULL;
            }
        }
    }

    /* Free only even-indexed survivors; deliberately leak odd-indexed ones */
    for (i = 0; i < 120; i += 2) {
        if (ptrs[i] != NULL) {
            free(ptrs[i]);
            ptrs[i] = NULL;
        }
    }

    printf("Exiting with some allocations unreleased (odd-indexed survivors).\n");
    printf("Leak detector on stderr should report leaked bytes and object count.\n");
    return EXIT_SUCCESS;
}
