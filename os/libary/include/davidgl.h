#ifndef DAVIDGL_H
#define DAVIDGL_H

#include <stdint.h>


typedef struct{
    int Quit = 1;

}Event;





void screen_clear();
void draw_rect(int x, int y, int w, int h, uint32_t color);
int is_pressed(char imp);



#endif