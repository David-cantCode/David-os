#ifndef DAVIDGL_H
#define DAVIDGL_H

#include <stdint.h>






void screen_clear();
void draw_rect(int x, int y, int w, int h, uint32_t color);
int is_pressed(char imp);
void draw_circle(int cx, int cy, int radius, uint32_t color);
void set_fps(unsigned int fps);
int should_update();

void draw_text(int x, int y, const char* str, uint32_t color, int size);
void flip();

#endif