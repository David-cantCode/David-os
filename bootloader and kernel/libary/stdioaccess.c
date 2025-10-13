


#include "stdioaccess.h"


    //Standard IO access libary 
    //used by PIC 







uint8_t inb(uint16_t port)
 {
uint8_t ret;
    __asm__ volatile ("inb %1, %0"
                      : "=a"(ret)
                      : "dN"(port));
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