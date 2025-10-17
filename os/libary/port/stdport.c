
#include "../include/stdport.h"

    //Standard port
   

uint8_t inb(uint16_t port)
 {
uint8_t ret;
    __asm__("in %%dx, %%al" : "=a" (ret) : "d" (port));
    return ret;
}


void outb(uint16_t port, uint8_t val){
    __asm__ volatile ("outb %0, %1"
                      :
                      : "a"(val), "dN"(port));
}


void io_wait(void)
{
    __asm__ volatile ("outb %%al, $0x80" : : "a"(0));
}


void insw(uint16_t port, void* addr, uint32_t count) {
    asm volatile ("rep insw" : "+D"(addr), "+c"(count) : "d"(port) : "memory");
}
