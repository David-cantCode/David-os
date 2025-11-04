#ifndef TERMINAL
#define TERMINAL

#include <stdint.h>
struct Window;
struct Terminal;


#define KEY_DOWN_SCANCODE_LIMIT 57
#define BACKSPACE 0x0e
#define ENTER 0x1C
#define SHIFT_KEY_P 0x2A

void terminal_print(struct Terminal* t,const char *string);

void terminal_update(struct Window* self);
void terminal_on_resize(struct Window* self_win, int new_width, int new_height);
void terminal_on_input(uint8_t scancode);

#endif


