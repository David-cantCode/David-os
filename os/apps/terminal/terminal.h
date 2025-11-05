#ifndef TERMINAL
#define TERMINAL

#include <stdint.h>
struct Window;

struct Terminal {
    struct Window* win;                  
    int cursor_row;
    int cursor_col;
      
    int num_lines;
    int control_row;
    uint16_t cur_dir_cluster;
    char cur_dir_name[8];

    char lines[128][128];
};


#define KEY_DOWN_SCANCODE_LIMIT 57
#define BACKSPACE 0x0e
#define ENTER 0x1C
#define SHIFT_KEY_P 0x2A

void terminal_print(struct Terminal* t,const char *string);

void terminal_update(struct Window* self);
void terminal_on_resize(struct Window* self_win, int new_width, int new_height);
void terminal_on_input(struct Window* win, uint8_t scancode);

#endif


