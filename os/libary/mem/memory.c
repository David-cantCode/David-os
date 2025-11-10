#include <stdint.h>



#include "../include/memory.h"



static uint8_t* heap_ptr = (uint8_t*)HEAP_START;
static uint8_t* heap_end = (uint8_t*)(HEAP_START + HEAP_SIZE);

void* memoryalloc(uint32_t size) {
    size = (size + 7) & ~7;  
    if (heap_ptr + size >= heap_end)
        return 0; 
    void* addr = heap_ptr;
    heap_ptr += size;
    return addr;
}