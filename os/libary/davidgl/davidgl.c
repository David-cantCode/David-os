#include "../include/stdconsole.h"



extern volatile int key_down; 
extern volatile uint8_t scancode;
extern const char scancode_to_char[];






//i know its silly but sue me 
void screen_clear(){
    clear_screen();
}   



void draw_rect(int x, int y, int w, int h, uint32_t color) {
    for (int row = 0; row < h; row++) {
        for (int col = 0; col < w; col++) {
            put_pixel(x + col, y + row, color);
        }
    }
}


int is_pressed(char imp){
    if (key_down == 0){return 0;}

    if (scancode_to_char[scancode] == imp){
        return 1;
    }


    return 0;
}


