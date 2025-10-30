#ifndef STDCONSOLE_H
#define STDCONSOLE_H


#include <stdint.h>
#define VGA_COLS 80 //x 
#define VGA_ROWS 25 // y 


#define VGA_ADDRESS 0xB8000 
static unsigned short* const vga = (unsigned short*)VGA_ADDRESS;

extern uint32_t fb_addr;  // filled in by kernel.asm
extern uint32_t pitch;
extern uint32_t screen_width;
extern uint32_t screen_height;


void print_string(char* string, unsigned char attribute, int posX, int posY);
void print_char(char character, unsigned char attribute, int posX, int posY);
static char* byte_to_hex(uint8_t b, char* buffer);


void put_pixel(int x, int y,uint32_t color);
void draw_char( int x, int y, uint32_t fg, uint32_t bg);


#endif