//process viewer
#include "../../libary/include/program.h"
#include "../../libary/include/util.h"
#include "../terminal/terminal.h"

#define MAX_PROGRAMS 32

extern struct Program* programs[MAX_PROGRAMS];
extern int program_count;



 struct Program* p;

 //huge ass bug tjats driving me crazy;
 //keeps printing termminal im gonna go insane lol prolly a problem with program.c/programs not being added correctly 
 //also i think some memalloc problems (here not in mem.c) cuz the font keeps getting messed 

void terminal_pviewer(struct Window* win){
    terminal_print(win, "\n");

    terminal_print(win, "Process Viewer\nID       NAME     SIZE\n========================\n \n");
    for(int i = 0; i < program_count; i++) {
    p = programs[i];
    
   
    if(p){

    //id
    char buffer[16];
    int_to_str(i, buffer);
    
    terminal_print(win, buffer);
    terminal_print(win, "    ");




    //name
    terminal_print(win, get_program_name(p->type));
    terminal_print(win, "      ");
   
    //size


    int_to_str(sizeof(*p), buffer);
    terminal_print(win, buffer);

     terminal_print(win, "\n");

    }

    }

    terminal_print(win, "\n");

}






