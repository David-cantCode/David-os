
#include "../include/program.h"
#include "../include/util.h"
#include "../include/stdconsole.h"
#include "../../apps/terminal/terminal.h"

extern Program* programs[MAX_PROGRAMS];
extern int program_count;


struct Program* create_program(enum Program_type type,
    void (*on_init)(Program*, Window*),
    void (*on_update)(Program*, Window*),
    void (*on_resize)(Program*, Window*, int, int),
    void (*on_input)(Program*, Window*, uint8_t),
    Window* win
    )

    {

    

    if (program_count >= MAX_PROGRAMS) {return 0;}
    
    Program* program = mem_alloc(sizeof(Program));

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
    print(get_program_name(program->type));
    //add to global programs


    //call on init if given 
   if (program->on_init)program->on_init(program, win);

    if (win){

        win->program.type = program->type;
        win->program.on_init = program->on_init;
        win->program.on_update = program->on_update;
        win->program.on_resize = program->on_resize;
        win->program.on_input = program->on_input;
        win->program.window = win;
        win->program.running = program->running;
    }

    
    return program;


}


char* get_program_name(enum Program_type type) {
    switch(type) {                  
        case PROGRAM_TERMINAL:  return "Terminal ";
        case PROGRAM_DSERVER:   return "Dserver  ";
        case PROGRAM_SHELL:     return "Shell    ";
        case PROGRAM_PONG:      return "Pong     ";




        default:                return "Unknown  ";
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






Program *p;

void terminal_pviewer(struct Window* win){

    terminal_print(win, "\n");

    terminal_print(win, "Process Viewer\nID       NAME     SIZE\n========================\n \n");
    for(int i = 0; i < program_count; i++) {
    p = programs[i];
    
   
    if(p){

    //id
    char buffer[16];
    int_to_str(i + 1, buffer);
    
    terminal_print(win, buffer);
    terminal_print(win, "       ");




    //name
    terminal_print(win, get_program_name(p->type));
    terminal_print(win, "  ");
   
    //size


    int_to_str(sizeof(*p), buffer);
    terminal_print(win, buffer);

     terminal_print(win, "\n");

    }

    }

    terminal_print(win, "\n");

}






void shell_pviewer(){
    print( "\n");

    print( "Process Viewer\nID       NAME     SIZE\n========================\n \n");
    for(int i = 0; i < program_count; i++) {
    p = programs[i];
    
   
    if(p){

    //id
    char buffer[16];
    int_to_str(i + 1, buffer);
    
    print(buffer);
    print( "       ");




    //name
    print(get_program_name(p->type));
    print("  ");
   
    //size


    int_to_str(sizeof(*p), buffer);
    print(buffer);

    print( "\n");

    }

    }

    print("\n");

}