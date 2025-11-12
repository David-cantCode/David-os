#ifndef STDCONSOLE_H
#define STDCONSOLE_H

#include "../../apps/dserver/dserver.h"
#include <stdint.h>



extern uint32_t fb_addr;  
extern uint32_t pitch;
extern uint32_t screen_width;
extern uint32_t screen_height;

void put_pixel(int x, int y, uint32_t color) ;
void draw_char(char c, int x, int y, uint32_t fg, uint32_t bg) ;



void control_line(int row);
void print(char *string);
void clear_screen();

void dump_sector(uint8_t *buf);
void print_backspace();

extern const char scancode_to_char[];

#endif