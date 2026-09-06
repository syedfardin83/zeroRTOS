#include "zeroRTOS_memory.h"

static uint8_t zrtos_heap[ZRTOS_TOTAL_HEAP_SIZE];
static uint8_t* nextFreeByte = zrtos_heap;

void* zrtos_malloc(size_t size){
    //  Check if enough space is available
    if(!(ZRTOS_TOTAL_HEAP_SIZE-(uint32_t)(nextFreeByte-zrtos_heap)>size)) return 0;

    uint8_t* ptr = nextFreeByte;
    nextFreeByte+=size;

    return (void*)ptr;
}

void* zrtos_memset(void* ptr, int c, size_t size){

    char* c_ptr = (char*)ptr;
    for(int i=0;i<size;i++){
        c_ptr[i]=(char)c;
    }
    return ptr;
}