#ifndef DSERVER_H
#define DSERVER_H

#include <stdint.h>



enum Program_type{
    PROGRAM_TERMINAL

};

struct Window; 

struct Program {
    void (*on_update)(struct Window* self);
    void (*on_resize)(struct Window* self, int new_width, int new_height);
    void (*on_input)(struct Window* win,uint8_t scancode);
    void (*on_init)(struct Window* self);

    enum Program_type type;
};



typedef struct Window {
    int x, y, width, height;
    int color;
    struct Program program; 




    uint16_t cur_dir_cluster;
    char cur_dir_name[8];


    char* buffer;
    char** lines;
    int* cursor_row;
    int* cursor_col;
    int* num_lines;
    int* control_row;

} Window;





void display_init();

#endif