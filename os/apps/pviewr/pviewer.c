//process viewer
#include "../../libary/include/program.h"
#include "../../libary/include/util.h"
#include "../terminal/terminal.h"

#define MAX_PROGRAMS 32

extern volatile struct Program* programs[MAX_PROGRAMS];
extern volatile int program_count;




void terminal_pviewer(struct Window* win){
    terminal_print(win, "\n");

    terminal_print(win, "Process Viewer\nID       NAME     SIZE\n========================\n \n");
    for(int i = 0; i < program_count; i++) {
    
    
    struct Program* p = programs[i];
    if(!p) continue;

    //id
    char* buffer;
    int_to_str(i, buffer);
    
    terminal_print(win, buffer);
    terminal_print(win, "    ");
    



    //name
    terminal_print(win, get_program_name(p->type));
    terminal_print(win, "      ");
   
    //size


    int_to_str(sizeof(p), buffer);
    terminal_print(win, buffer);

     terminal_print(win, "\n");

    }

    terminal_print(win, "\n");
}






