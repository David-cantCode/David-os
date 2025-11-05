#ifndef MEMORY_H
#define MEMORY_H


#include <stdint.h>


#define HEAP_START 0x00200000   // mem addr start (2mb) im 67% sure this would not interfer with kernel considering KERNEL_START_ADDR   equ 0x00100000       
#define HEAP_SIZE  0x00800000  


void* memoryalloc(uint32_t size);

#endif