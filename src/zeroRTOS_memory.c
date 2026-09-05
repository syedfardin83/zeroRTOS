#include "zeroRTOS_memory.h"

static uint8_t zrtos_heap[ZRTOS_TOTAL_HEAP_SIZE];
static uint8_t* nextFreeByte = zrtos_heap;

void* zrtos_malloc(size_t size){
    
}

void zrtos_memory_init(){

}