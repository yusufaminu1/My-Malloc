#include <stdio.h>
#define MEMLENGTH 4096
static union {
    char bytes[MEMLENGTH];
    double not_used;
} heap;
static int intialized = 0;
typedef struct{
    int size;
    int allocated;
} chunk_header;
static size_t round8(size_t sz) {
    return (sz + 7) & ~(size_t)7; 
}
static void leak_detector(){
    chunk_header *chunk = heap.bytes;
    int objects = 0;
    int size = 0;
    while(1){
        if(chunk->allocated=1){
            objects++;
            size+=chunk->size;
        }
        chunk = next_chunk(chunk);
        if(!inside_heap){
            break;
        }
    }
    if(size>0 ||objects>0){
        fprintf(stderr, "mymalloc: %d bytes leaked in %d objects.\n", size, objects);
    }
}
void initialize_heap(){
    chunk_header init = {MEMLENGTH-sizeof(chunk_header),0};
    *(chunk_header *)heap.bytes = init;
    atexit(leak_detector);
    intialized = 1;
}
static chunk_header *next_chunk(chunk_header *h){
    return (chunk_header *)((sizeof(chunk_header)+h->size) + (char *)h);
}
static int inside_heap(chunk_header *h){
    if((char *)h > heap.bytes && (char *)h<heap.bytes+MEMLENGTH){
        return 1;
    }else{
        return 0;
    }
}
void *mymalloc(size_t size, char *file, int line){
    if(!intialized) initialize_heap();
    if(size == 0) return NULL;
    size = round8(size);
    chunk_header *chunk = heap.bytes;
    while(1){
        if(chunk->allocated == 0 && chunk->size >= size){
            if(chunk->size -size >= 16){
                int newSize = chunk->size - (sizeof(chunk_header)+size);
                chunk_header newChunk = {newSize, 0};
                *(chunk_header *)((char *)chunk + size +sizeof(chunk_header)) = newChunk;
                chunk->size = size;
                chunk->allocated = 1;
                return (char *)(chunk) +sizeof(chunk_header);
            }else{
                chunk->size = size;
                chunk->allocated = 1;
                return (char *)(chunk) +sizeof(chunk_header);
            }
        }else{
          chunk = next_chunk(chunk);
          if(!inside_heap(chunk)){
            fprintf(stderr, "malloc: Unable to allocate %zu bytes (%s:%d)\n", size, file, line);
            return NULL;
          }
        }
    }
}
void   myfree (void *ptr, char *file, int line){

}