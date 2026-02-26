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


    for (i = 0; i < 120; i++) {
        if (ptrs[i] != NULL) {
            free(ptrs[i]);
            ptrs[i] = NULL;
        }
    }

    printf("PASS test_randomnoleak (leak detector should report nothing on stderr)\n");
    return EXIT_SUCCESS;
}
