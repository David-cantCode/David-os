#include "terminal.h"

#include "../../libary/include/davidgl.h"
#include "../../libary/include/util.h"
#include "../dserver/window.h"

struct Terminal {
    struct Window* win;                  
    int cursor_row;
    int cursor_col;
    char lines[128][128];            
    int num_lines;
};


struct Terminal self;


void run_cmd(){


}

void terminal_on_resize(struct Window* self_win, int new_width, int new_height){
    self.win = self_win; 

    
}



void terminal_on_input(char key){
}


void terminal_draw(struct Window* win){


    int c_width = 8;   
    int c_height = 16; 

    for (int row = 0; row < self.num_lines; row++) {
    for (int col = 0; col < 128; col++) {
        char c = self.lines[row][col];
        if (c != '\0') {
            //set pos relative to win
        int x = win->x + col * c_width;
        int y = win->y + row * c_height;

        
        //set bounds
        if (x + c_width <= win->x + win->width && y + c_height <= win->y + win->height){
        d_char(x, y, c, 0xFFFFFFFF, 1);}
            
        }}}
}


void terminal_update(struct Window* win) {
    self.win = win;
    self.num_lines = 6; 
    self.lines[1][1] = 'h';
    self.lines[1][2] = 'i';


    terminal_draw(win);
}