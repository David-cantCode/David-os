#include "terminal.h"

#include "../../libary/include/davidgl.h"
#include "../../libary/include/util.h"
#include "../dserver/dserver.h"

#include "../fetch/fetch.h"


struct Terminal self;
#define CHAR_W 12
#define CHAR_H 16


extern int shift_down;
extern const char scancode_to_char[];
//ret num cols and rows to fit win
int terminal_fb_cols(struct Terminal* t) {
    if (self.win && t->win->width > 0) return t->win->width / CHAR_W;
    return 80;
}
int terminal_fb_rows(struct Terminal* t) {
    if (t->win && t->win->height > 0) return t->win->height / CHAR_H;
    return 25;
}

void terminal_control_line(struct Terminal* t, int row) {
    t->control_row = row;
    if (t->control_row < 0) t->control_row = 0;
    if (t->control_row >= terminal_fb_rows(t)) t->control_row = terminal_fb_rows(t) - 1;
}



void terminal_set_char_at_video_memory(struct Terminal* t, char c, int col, int row) {
    int cols = terminal_fb_cols(t);
    int rows = terminal_fb_rows(t);

    // bounds check
    if (row < 0 || row >= rows) return;
    if (col < 0 || col >= cols) return;

    t->lines[row][col] = c;

    // calculate draw position
    int x = t->win->x + col * CHAR_W;
    int y = t->win->y + row * CHAR_H;

    d_char(x, y, c, 0xFFFFFFFF, 1);

    t->cursor_col = col;
    t->cursor_row = row;
}

void terminal_draw(struct Window* win){
    //redraws the terminal each call


    int c_width = 8;   
    int c_height = 16; 

    for (int row = 0; row < self.num_lines; row++) {
    for (int col = 0; col < 128; col++) {
        char c = self.lines[row][col];
        if (c != '\0') {
            //set pos relative to win
        int x = win->x + col * c_width +5;
        int y = win->y + row * c_height +10 ;

        
        //set bounds
        if (x + c_width <= win->x + win->width && y + c_height  <= win->y + win->height + 100){ //note the added ints are for padding
        d_char(x, y, c, 0xFFFFFFFF, 1);}
            
        }}}
}

void terminal_scroll_screen(struct Terminal* t) {
    int rows = terminal_fb_rows(t);
    int cols = terminal_fb_cols(t);

    for (int r = t->control_row; r < t->num_lines - 1; ++r) {
        memorycpy(t->lines[r], t->lines[r + 1], 128);
    }
    memoryset(t->lines[t->num_lines - 1], 0, 128);


   
}


void terminal_print(struct Terminal* t, const char *string) {
    int cols = terminal_fb_cols(t);
    int rows = terminal_fb_rows(t);

    int cur_row = t->cursor_row;
    int cur_col = t->cursor_col;

    for (int i = 0; string[i] != '\0'; i++) {
        char c = string[i];

        if (c == '\n') {
            cur_row++;
            cur_col = 0;
            if (cur_row >= rows) {
                terminal_scroll_screen(t);
                cur_row = rows - 1;
            }
        } else {
            terminal_set_char_at_video_memory(t,c, cur_col, cur_row);

            cur_col++;
            if (cur_col >= cols) {
                cur_col = 0;
                cur_row++;
                if (cur_row >= rows) {
                    terminal_scroll_screen(t);
                    cur_row = rows - 1;
                }
            }
        }
    }

    t->cursor_row = cur_row;
    t->cursor_col = cur_col;
}

void terminal_print_backspace(struct Terminal* t ) {
    if (self.cursor_col > 0) {
        self.cursor_col--;
    } else if (self.cursor_row > 0) {
        self.cursor_row--;
        self.cursor_col = terminal_fb_cols(t) - 1;
    }

    terminal_set_char_at_video_memory(t, ' ', self.cursor_col, self.cursor_row);
}

void terminal_clear(struct Terminal* t){
    int cols = terminal_fb_cols(t);
    int rows = terminal_fb_rows(t);
    for (int r = t->control_row; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
      
        terminal_set_char_at_video_memory(t, ' ', c, r);
        }
    }
   t->cursor_row =0; t->cursor_col =0;
}

void run_cmd(){


}

void terminal_on_resize(struct Window* self_win, int new_width, int new_height){
    self.win = self_win; 

    self.num_lines = terminal_fb_rows(&self);
}

int terminal_backspace(char buffer[]) {
    int len = string_length(buffer);
    if (len > 0) {
        buffer[len - 1] = '\0';
        return 1;
    } else {
        return 0;
    }
}

void terminal_execute_command(char *input){


    if (compare_string(input, "fetch")==0){
        t_fetch(&self);
    }

    else if(compare_string(input, "clear")==0)
    {
        terminal_clear(&self);
    }

    else {

    terminal_print(&self, "unknown command '");
    terminal_print(&self, input);
    terminal_print(&self, "' \ntype 'help' for info\n \n");
    }

}






void terminal_on_input(uint8_t scancode){
    //runs when key is pressed
    if (scancode > KEY_DOWN_SCANCODE_LIMIT) return;


    //deafult commands, backspace n enter
    if (scancode == BACKSPACE) {
       if (terminal_backspace(self.buffer)) terminal_print_backspace(&self);
    }

    else if (scancode == ENTER) {
        terminal_print(&self, "\n");
        terminal_execute_command(self.buffer);
        self.buffer[0] = '\0';


        if (self.cur_dir_cluster == 0){
        memoryset(self.cur_dir_name, '~', 1); //fix to delete spaces inbetween ~ and/ 
        }
        terminal_print(&self,self.cur_dir_name);
        terminal_print(&self, "/ $ ");

    }

    else if (scancode == SHIFT_KEY_P){ //so we dont show ? when shift pressed
    }

    else{
        char letter = scancode_to_char[(int) scancode];

        //freaky ah way of shift control, im not compaling tho
        if (shift_down && letter >= 'a' && letter <= 'z') letter -= 32;
        else if (!shift_down && letter >= 'A' && letter <= 'Z') letter += 32;


         append(self.buffer, letter);
        char str[2] = {letter, '\0'};
        terminal_print(&self, str);
    }





}





void terminal_update(struct Window* win) {
    self.win = win;

    terminal_draw(win);//redraw each frame
}