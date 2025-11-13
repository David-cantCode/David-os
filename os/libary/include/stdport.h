#ifndef STDIOACESS_H
#define STDIOACESS_H

#include <stdint.h>



 uint8_t inb(uint16_t port);
 void outb(uint16_t port, uint8_t val);
 void io_wait(void);
void insw(uint16_t port, void* addr, uint32_t count);
uint16_t inw(uint16_t port);
void outsw(uint16_t port, const void* buffer, uint32_t count);
uint16_t htons(uint16_t x);
#endif