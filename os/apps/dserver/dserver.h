#ifndef DSERVER_H
#define DSERVER_H


#include "window.h"
#include <stdint.h>



enum Program_type{
    PROGRAM_TERMINAL

};

struct Window; 
struct Program {
    void (*on_update)(struct Window* self);
    void (*on_resize)(struct Window* self, int new_width, int new_height);
    void (*on_input)(uint8_t scancode);

    enum Program_type type;
};



typedef struct Window {
    int x, y, width, height;
    int color;
    struct Program program; 
} Window;





void display_init();

#endif