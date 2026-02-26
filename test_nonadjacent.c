#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"


int main() {
   
    char *p[4];
    int i;

    for (i = 0; i < 4; i++) {
        p[i] = malloc(1016);
        if (p[i] == NULL) {
            printf("FAIL: malloc returned NULL on initial allocation %d\n", i);
            return EXIT_FAILURE;
        }
    }

    free(p[0]); 
    free(p[3]); 

   
    void *big = malloc(1500);
    if (big != NULL) {
        printf("FAIL: malloc returned non-NULL for 1500 bytes when only"
               " two non-adjacent 1016-byte free blocks exist\n");
        free(big);
        free(p[1]);
        free(p[2]);
        return EXIT_FAILURE;
    }

    free(p[1]);
    free(p[2]);
    printf("PASS test_nonadjacent (malloc correctly returned NULL, error on stderr)\n");
    return EXIT_SUCCESS;
}
