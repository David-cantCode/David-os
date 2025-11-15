
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


uint16_t inw(uint16_t port) {
    uint16_t ret;
    __asm__ volatile ("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void outsw(uint16_t port, const void* buffer, uint32_t count) {
    const uint16_t* buf = (const uint16_t*)buffer;
    __asm__ volatile ("cld\n\t" "rep outsw": "+S"(buf), "+c"(count) : "d"(port): "memory");
}


uint16_t htons(uint16_t x) {
    return (x << 8) | (x >> 8);
}

 void outl(uint16_t port, uint32_t val) {
    __asm__ volatile ("outl %0, %1" : : "a"(val), "Nd"(port));
}

 uint32_t inl(uint16_t port) {
    uint32_t ret;
    __asm__ volatile ("inl %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}
