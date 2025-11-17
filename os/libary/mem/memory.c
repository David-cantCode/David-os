#include <stdint.h>


#include "../include/util.h"
#include "../include/memory.h"
#include "../include/stdconsole.h"


uint8_t* heap_ptr;
uint8_t* heap_end;

//note fix memory use mem to check info and check linker too 

typedef struct {
    uint32_t size;
} BlockHeader;


void memory_init() {
    heap_ptr = (uint8_t*)&__heap_start;
    heap_end = (uint8_t*)&__heap_end;
}


void* mem_alloc(uint32_t size) {
    if (heap_ptr + sizeof(BlockHeader) + size >= heap_end)
        return 0;

    BlockHeader* header = (BlockHeader*)heap_ptr;
    header->size = size;
    void* addr = heap_ptr + sizeof(BlockHeader);
    heap_ptr += sizeof(BlockHeader) + size;
    return addr;
}

void* mem_realloc(void* ptr, uint32_t new_size) {
    if (!ptr) 
        return mem_alloc(new_size);
    if (new_size == 0) 
        return 0;

    BlockHeader* header = (BlockHeader*)((uint8_t*)ptr - sizeof(BlockHeader));
    uint32_t old_size = header->size;

    void* new_ptr = mem_alloc(new_size);
    if (!new_ptr)
        return 0;

    //copy old data to the new block
    uint32_t copy_size = old_size < new_size ? old_size : new_size;
    memorycpy(new_ptr, ptr, copy_size);

    return new_ptr;
}




void shell_mem() {
    //prints mem status for shell

    //idk im bored lol

    uint32_t total = (uint32_t)(heap_end - (uint8_t*)HEAP_START);
    uint32_t used  = (uint32_t)(heap_ptr - (uint8_t*)HEAP_START);
    uint32_t free  = total - used;

    char buf[128];

    print("Memory Status:\n");

    s_printf(buf, "  Heap Start: 0x%x\n", (uint32_t)HEAP_START);
    print(buf);

    s_printf(buf, "  Heap End:   0x%x\n", (uint32_t)heap_end);
    print(buf);

    s_printf(buf, "  Heap Ptr:   0x%x\n", (uint32_t)heap_ptr);
    print(buf);

    s_printf(buf, "  Total:      %d bytes\n", total);
    print(buf);

    s_printf(buf, "  Used:       %d bytes\n", used);
    print(buf);

    s_printf(buf, "  Free:       %d bytes\n", free);
    print(buf);
}
