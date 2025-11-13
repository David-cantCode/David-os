#ifndef PROGRAM_H
#define PROGRAM_H

#include "../../libary/include/memory.h"
#include <stdint.h>

#define MAX_PROGRAMS 32


enum Program_type{
    PROGRAM_TERMINAL,
    PROGRAM_DSERVER,
    PROGRAM_PONG
};

struct Window; 



//for programs without windows like in the shell just pass 0
typedef struct Program {
    void (*on_update)(struct Program* self, struct Window* win);
    void (*on_resize)(struct Program* self, struct Window* win, int new_width, int new_height);
    void (*on_input)(struct Program* self, struct Window* win, uint8_t scancode);
    void (*on_init)(struct Program* self, struct Window* win);


    struct Window* window; 
    int running;
    enum Program_type type;
}Program;



typedef struct Window {
    int x, y, width, height;
    int color;
    struct Program program; 
    
    
    char buffer[256];


    //for terminal related stuff 
    int cursor_row;
    int cursor_col;  
    int num_lines;
    int control_row;
    uint16_t cur_dir_cluster;
    char cur_dir_name[8];
    char lines[128][128];

} Window;




Program* create_program(enum Program_type type,
    void (*on_init)(Program*, Window*),
    void (*on_update)(Program*, Window*),
    void (*on_resize)(Program*, Window*, int, int),
    void (*on_input)(Program*, Window*, uint8_t),
    Window* window 
    );

char* get_program_name(enum Program_type type) ;
int get_max_programs();
Program* get_programs();
int get_program_count();
#endif