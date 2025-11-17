#ifndef MEMORY_H
#define MEMORY_H


#include <stdint.h>

extern uint32_t __heap_start;
extern uint32_t __heap_end;

#define HEAP_START ((uint32_t)&__heap_start)
#define HEAP_END   ((uint32_t)&__heap_end)
#define HEAP_SIZE  (HEAP_END - HEAP_START)

//im so silly turns out my old heap was mapped to nothing lol


void memory_init();
void* mem_alloc(uint32_t size);
void* mem_realloc(void* ptr, uint32_t new_size) ;

void shell_mem();
#endif