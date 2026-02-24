#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "mymalloc.h"
typedef struct Node {
  int data;
  struct Node *next;
} Node;

int main() {
  struct timeval start, end;
  long elapsed;
  srand(time(NULL));

  //task one
  gettimeofday(&start, NULL);
  for (int j = 0; j < 50; j++) {
    for(int i = 0; i < 120; i++) {
      char *oneByte = malloc(1);  
      free(oneByte);
    }
  }
  gettimeofday(&end, NULL);
  elapsed = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
  printf("Average time for task 1: %f microseconds", (double)elapsed / 50);

  //task two
  gettimeofday(&start, NULL);
  for(int j = 0; j < 50; j++) {
    char *arr[120];
    for(int i = 0; i < 120; i++) {
      arr[i] = malloc(1);
    }
    for(int i = 0; i < 120; i++) {
      free(arr[i]);
    }
  }
  gettimeofday(&end, NULL);
  elapsed = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
  printf("\nAverage time for task 2: %f microseconds", (double)elapsed / 50);
    

  //task three
  gettimeofday(&start, NULL);
  for(int j = 0; j < 50; j++) {
    char *arrTwo[120] = {NULL};
    int allocations = 0;
    while (allocations != 120) {
      int random = rand() % 2;
      if(random == 0) {
        char *oneByte = malloc(1);
        arrTwo[allocations] = oneByte;
        allocations++;
      } else {
        if (allocations > 0) {
          int randomFree = rand() % allocations;
          if(arrTwo[randomFree] != NULL) {
            free(arrTwo[randomFree]);
            arrTwo[randomFree] = NULL;
          }
        }
      }
    }
    for(int i = 0; i < 120; i++) {
      if(arrTwo[i] != NULL) { free(arrTwo[i]); }
    }
  }
  gettimeofday(&end, NULL);
  elapsed = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
  printf("\nAverage time for task 3: %f microseconds", (double)elapsed / 50);

  //task four
  gettimeofday(&start, NULL);
  for(int j = 0; j < 50; j++) {
    Node *head = NULL;
    Node *curr = NULL;

    for(int i = 0; i < 30; i++) {
      Node *newNode = malloc(sizeof(Node));
      newNode->data = i;
      newNode->next = NULL;
      if(head == NULL) {
        head = newNode;
        curr = head;
      } else {
        curr->next = newNode;
        curr = curr->next;
      }
    }

    curr = head;
    while(curr != NULL) {
      Node *temp = curr->next;
      free(curr);
      curr = temp;
    }
    head = NULL;
  }
  gettimeofday(&end, NULL);
  elapsed = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
  printf("\nAverage time for task 4: %f microseconds", (double)elapsed / 50);

  //task five
  gettimeofday(&start, NULL);
  for(int j = 0; j < 50; j++) {
    char **matrix = malloc(20 * sizeof(char *));
    for(int i = 0; i < 20; i++) {
      matrix[i] = malloc(20 * sizeof(char));
    }

    for(int row = 0; row < 20; row++) {
      for(int col = 0; col < 20; col++) {
        matrix[row][col] = row + col;
      }
    }

    for(int i = 0; i < 20; i++) {
      free(matrix[i]);
    }
    free(matrix);
  }
  gettimeofday(&end, NULL);
  elapsed = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
  printf("\nAverage time for task 5: %f microseconds\n", (double)elapsed / 50);

  return 0;
}

