#include "terminal.h"

#include "../../libary/include/davidgl.h"
#include "../../libary/include/util.h"
#include "../dserver/dserver.h"

struct Terminal {
    struct Window* win;                  
    int cursor_row;
    int cursor_col;
    char lines[128][128];            
    int num_lines;
    int control_row;
};


struct Terminal self;
#define CHAR_W 8
#define CHAR_H 16


char buf[256]; //change to in struct afetwards

//ret num cols and rows to fit win
int terminal_fb_cols() {
    if (self.win && self.win->width > 0) return self.win->width / CHAR_W;
    return 80;
}
int terminal_fb_rows() {
    if (self.win && self.win->height > 0) return self.win->height / CHAR_H;
    return 25;
}

void terminal_control_line(int row) {
    self.control_row = row;
    if (self.control_row < 0) self.control_row = 0;
    if (self.control_row >= terminal_fb_rows()) self.control_row = terminal_fb_rows() - 1;
}



void terminal_set_char_at_video_memory(char c, int col, int row) {
    int cols = terminal_fb_cols();
    int rows = terminal_fb_rows();

    // bounds check
    if (row < 0 || row >= rows) return;
    if (col < 0 || col >= cols) return;

    self.lines[row][col] = c;

    // calculate draw position
    int x = self.win->x + col * CHAR_W;
    int y = self.win->y + row * CHAR_H;

    d_char(x, y, c, 0xFFFFFFFF, 1);

    self.cursor_col = col;
    self.cursor_row = row;
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

void terminal_scroll_screen() {
    int rows = terminal_fb_rows();
    int cols = terminal_fb_cols();

    for (int r = self.control_row; r < self.num_lines - 1; ++r) {
        memorycpy(self.lines[r], self.lines[r + 1], 128);
    }
    memoryset(self.lines[self.num_lines - 1], 0, 128);


    terminal_draw(self.win);
}


void terminal_print(const char *string) {
    int cols = terminal_fb_cols();
    int rows = terminal_fb_rows();

    int cur_row = self.cursor_row;
    int cur_col = self.cursor_col;

    for (int i = 0; string[i] != '\0'; i++) {
        char c = string[i];

        if (c == '\n') {
            cur_row++;
            cur_col = 0;
            if (cur_row >= rows) {
                terminal_scroll_screen();
                cur_row = rows - 1;
            }
        } else {
            terminal_set_char_at_video_memory(c, cur_col, cur_row);

            cur_col++;
            if (cur_col >= cols) {
                cur_col = 0;
                cur_row++;
                if (cur_row >= rows) {
                    terminal_scroll_screen();
                    cur_row = rows - 1;
                }
            }
        }
    }

    self.cursor_row = cur_row;
    self.cursor_col = cur_col;
}



void run_cmd(){


}

void terminal_on_resize(struct Window* self_win, int new_width, int new_height){
    self.win = self_win; 

    self.num_lines = terminal_fb_rows();
}



void terminal_on_input(uint8_t scancode){
    terminal_print("typing");
    

    

}





void terminal_update(struct Window* win) {
    self.win = win;

    terminal_draw(win);
}