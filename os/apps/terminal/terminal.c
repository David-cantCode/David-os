#include "terminal.h"

#include "../../libary/include/davidgl.h"
#include "../../libary/include/util.h"
#include "../dserver/dserver.h"
#include "t_cmds.h"


#define CHAR_W 12
#define CHAR_H 16

extern int shift_down;
extern const char scancode_to_char[];

//ret num cols and rows to fit win
int terminal_fb_cols(struct Window* win) {
    if (win && win->width > 0) return win->width / CHAR_W;
    return 80;
}

int terminal_fb_rows(struct Window* win) {
    if (win && win->height > 0) return win->height / CHAR_H;
    return 25;
}

void terminal_control_line(struct Window* win, int row) {
    win->control_row = row;
    if (win->control_row < 0) win->control_row = 0;
    if (win->control_row >= terminal_fb_rows(win)) win->control_row = terminal_fb_rows(win) - 1;
}

void terminal_set_char_at_video_memory(struct Window* win, char c, int col, int row) {
    int cols = terminal_fb_cols(win);
    int rows = terminal_fb_rows(win);

    // bounds check
    if (row < 0 || row >= rows) return;
    if (col < 0 || col >= cols) return;

    win->lines[row][col] = c;

    // calculate draw position
    int x = win->x + col * CHAR_W;
    int y = win->y + row * CHAR_H;

    d_char(x, y, c, 0xFFFFFFFF, 1);

    win->cursor_col = col;
    win->cursor_row = row;
}

void terminal_draw(struct Window* win) {
    int c_width = 8;   
    int c_height = 16; 

    for (int row = 0; row < win->num_lines; row++) {
        for (int col = 0; col < 128; col++) {
            char c = win->lines[row][col];
            if (c != '\0') {
                int x = win->x + col * c_width + 5;
                int y = win->y + row * c_height + 10;

                if (x + c_width <= win->x + win->width && y + c_height <= win->y + win->height + 100) {
                    d_char(x, y, c, 0xFFFFFFFF, 1);
                }
            }
        }
    }
}

void terminal_scroll_screen(struct Window* win) {
    int rows = terminal_fb_rows(win);
    int cols = terminal_fb_cols(win);

    for (int r = win->control_row; r < win->num_lines - 1; ++r) {
        memorycpy(win->lines[r], win->lines[r + 1], 128);
    }
    memoryset(win->lines[win->num_lines - 1], 0, 128);
}

void terminal_print(struct Window* win, const char *string) {
    int cols = terminal_fb_cols(win);
    int rows = terminal_fb_rows(win);

    int cur_row = win->cursor_row;
    int cur_col = win->cursor_col;

    for (int i = 0; string[i] != '\0'; i++) {
        char c = string[i];

        if (c == '\n') {
            cur_row++;
            cur_col = 0;
            if (cur_row >= rows) {
                terminal_scroll_screen(win);
                cur_row = rows - 1;
            }
        } else {
            terminal_set_char_at_video_memory(win, c, cur_col, cur_row);

            cur_col++;
            if (cur_col >= cols) {
                cur_col = 0;
                cur_row++;
                if (cur_row >= rows) {
                    terminal_scroll_screen(win);
                    cur_row = rows - 1;
                }
            }
        }
    }

    win->cursor_row = cur_row;
    win->cursor_col = cur_col;
}

void terminal_print_backspace(struct Window* win) {
    if (win->cursor_col > 0) {
        win->cursor_col--;
    } else if (win->cursor_row > 0) {
        win->cursor_row--;
        win->cursor_col = terminal_fb_cols(win) - 1;
    }

    terminal_set_char_at_video_memory(win, ' ', win->cursor_col, win->cursor_row);
}

void terminal_clear(struct Window* win) {
    int cols = terminal_fb_cols(win);
    int rows = terminal_fb_rows(win);
    for (int r = win->control_row; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            terminal_set_char_at_video_memory(win, ' ', c, r);
        }
    }
    win->cursor_row = 0;
    win->cursor_col = 0;
}

void terminal_on_resize(struct Window* win, int new_width, int new_height) {
    win = win;
    win->num_lines = terminal_fb_rows(win);
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



void terminal_on_input(struct Window* win, uint8_t scancode) {

    if (scancode == BACKSPACE) {
       if (terminal_backspace(win->buffer)) terminal_print_backspace(win);
    }

    else if (scancode == ENTER) {


        
        terminal_execute_command(win,win->buffer);
        win->buffer[0] = '\0';


        if (win->cur_dir_cluster == 0){
        memoryset(win->cur_dir_name, '~', 1); //fix to delete spaces inbetween ~ and/ 
        }
        terminal_print(win,win->cur_dir_name);
        terminal_print(win, "/ $ ");
        
    }

    else if (scancode == SHIFT_KEY_P){ //so we dont show ? when shift pressed
    }

    else{
       char letter = scancode_to_char[(int) scancode];
        //freaky ah way of shift control, im not compaling tho
        if (shift_down && letter >= 'a' && letter <= 'z') letter -= 32;
        else if (!shift_down && letter >= 'A' && letter <= 'Z') letter += 32;


        
    
        append(win->buffer, letter, 256);
  

        char str[2] = {letter, '\0'};
        terminal_print(win, str);
    }
}

void terminal_update(struct Window* win) {

    terminal_draw(win);
}
