#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"



int main() {

    char *p[4];
    int i;

    for (i = 0; i < 4; i++) {
        p[i] = malloc(1016);
        if (p[i] == NULL) {
            printf("FAIL: malloc returned NULL on allocation %d\n", i);
            return EXIT_FAILURE;
        }
    }

    
    free(p[0]);
    free(p[2]);
    free(p[1]); 
    free(p[3]); 

    char *all = malloc(4088);
    if (all == NULL) {
        printf("FAIL: could not allocate 4088 bytes after non-sequential frees"
               " (coalescing failed)\n");
        return EXIT_FAILURE;
    }
    free(all);
    printf("PASS test_coalesce_order\n");
    return EXIT_SUCCESS;
}
