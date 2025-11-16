#ifndef DAVIDGL_H
#define DAVIDGL_H

#include <stdint.h>






void screen_clear();
void draw_rect(int x, int y, int w, int h, uint32_t color);
int is_pressed(char imp);
void draw_circle(int cx, int cy, int radius, uint32_t color);
void set_fps(unsigned int fps);
int should_update();


void d_char(int x, int y, char c, uint32_t color, int size);
void draw_text(int x, int y, const char* str, uint32_t color, int size);
void draw_pixel(int x, int y, uint32_t color);
void draw_line(int x0, int y0, int x1, int y1, uint32_t color);
void flip();
uint8_t char_to_scancode(char c);
#endif