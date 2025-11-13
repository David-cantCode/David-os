
#include "../include/program.h"
#include "../include/util.h"




struct Program* programs[MAX_PROGRAMS];
int program_count;


struct Program* create_program(enum Program_type type,
    void (*on_init)(Program*, Window*),
    void (*on_update)(Program*, Window*),
    void (*on_resize)(Program*, Window*, int, int),
    void (*on_input)(Program*, Window*, uint8_t),
    Window* win
    )

    {

    

    if (program_count >= MAX_PROGRAMS) {return 0;}
    
    Program* program = memoryalloc(sizeof(Program));

    memoryset(program, 0, sizeof(Program));

    //ini variables
    program->type = type;
    program->on_init = on_init;
    program->on_update = on_update;
    program->on_resize = on_resize;
    program->on_input = on_input;
    program->window = win;
    program->running = 1;


    

    programs[program_count++] = program; 
    //add to global programs


    //call on init if given 
    if (program->on_init)program->on_init(program, win);

    if (win){
        win->program = *program;
    }

    
    return program;


}


char* get_program_name(enum Program_type type) {
    switch(type) {
        case PROGRAM_TERMINAL: return "Terminal";
        case PROGRAM_DSERVER: return "Dserver";
        default: return "Unknown";
    }
}


int get_max_programs(){
    return MAX_PROGRAMS;
}


Program* get_programs(){
    return *programs;
}

int get_program_count(){
    return program_count;
}