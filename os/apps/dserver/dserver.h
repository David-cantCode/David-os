#ifndef DSERVER_H
#define DSERVER_H


#include "window.h"
#include <stdint.h>





enum Program_type{
    PROGRAM_TERMINAL

};



struct Program {
    void (*on_update)(struct Window* self);
    void (*on_resize)(struct Window* self, int new_width, int new_height);
    void (*on_input)(char key);

    enum Program_type type;
};




void display_init();

#endif