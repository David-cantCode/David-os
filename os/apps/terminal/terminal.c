#include "terminal.h"

#include "../../libary/include/stdconsole.h"
#include "../../libary/include/davidgl.h"

char *buffer[256];
int width;
int height;

Terminal self;

void run_cmd(){


}

void terminal_on_resize(struct Window* self_win, int new_width, int new_height){
    self.win = self_win; 


    terminal_print(&self, "Hello world");

}



void terminal_on_input(char key){
}



void terminal_update(struct Window* self_win){

    draw_rect(50, 50, 30, 40, 0xFFFFFFFF);
}