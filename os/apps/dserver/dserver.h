#ifndef DSERVER_H
#define DSERVER_H


#include <stdint.h>

enum Program_type{
    PROGRAM_SHELL

};



struct Program {
    void (*on_update)();
    void (*on_resize)(int new_width, int new_height);
    void (*on_input)(char key);

    enum Program_type type;
};



typedef struct {
    int x, y;
    int width, height;
    uint32_t color;

    struct Program* program;
} Window;

void display_init();

#endif