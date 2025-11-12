
#include "../fetch/fetch.h"
#include "terminal.h"
#include "../../libary/include/util.h"
#include "../../apps/help/help.h"
#include "../../libary/include/fat16.h"
#include "../../libary/include/stdconsole.h"
#include "../../apps/dserver/dserver.h"
#include "../pviewr/pviewer.h"



void terminal_execute_command(struct Window* win, char *input) {

    //**********************
    //*******GENERAL********
    //**********************
    if (compare_string(input, "clear") == 0) {

        terminal_clear(win);

    }
    
    else if (starts_with(input, "mkdir ")) {
    char* name = input + 6;
    name = conv_fat_name(name);

    create_directory(name, win->cur_dir_cluster, win->cur_dir_cluster == 0);
    terminal_print(win, "Created Directory: ");
    terminal_print(win, name);
    terminal_print(win, "\n \n");

    }


    else if (compare_string(input, "ls") == 0) {
    list_terminal_directory(win, win->cur_dir_cluster);
    terminal_print(win, "\n");
    }

    else if (starts_with(input, "cd ")) {
        char* name = input + 3;
     if (*name == '.' && name[1] == '\0') {return;}

        name = conv_fat_name(name);

        uint16_t new_cluster = find_directory_cluster(name, win->cur_dir_cluster);

        if (new_cluster != 0xFFFF) {
            win->cur_dir_cluster = new_cluster;
            terminal_print(win,"Entered directory: ");
            terminal_print(win,name);
            memorycpy(win->cur_dir_name, name, 8);


            terminal_print(win,"\n");}
        else {
            terminal_print(win,"Directory '");
            terminal_print(win, input +3);
            terminal_print(win,"' not found.\n");}
                
    }


    //**********************
    //**********CLIS********
    //**********************
    else if (compare_string(input, "fetch") == 0) {
        t_fetch(win);
    } 

    else if (compare_string(input, "pviewer") == 0) {
        terminal_pviewer(win);
    } 


    
    
    
    
    else {

        terminal_print(win, "\nunknown command '");
        terminal_print(win, input);
        terminal_print(win, "' \ntype 'help' for info\n \n");
    }


}