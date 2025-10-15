#ifndef STDCONSOLE_H
#define STDCONSOLE_H


#include <stdint.h>
#define VGA_COLS 80 //x 
#define VGA_ROWS 25 // y 


#define VGA_ADDRESS 0xB8000 
static unsigned short* const vga = (unsigned short*)VGA_ADDRESS;


int string_length(char s[]);
void print_string(char* string, unsigned char attribute, int posX, int posY);
void print_char(char character, unsigned char attribute, int posX, int posY);
static char* byte_to_hex(uint8_t b, char* buffer);
void clear_screen();
void append(char s[], char n);
#endif