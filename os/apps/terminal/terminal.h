#ifndef TERMINAL
#define TERMINAL

struct Window;
struct Terminal;

void terminal_update(struct Window* self);
void terminal_on_resize(struct Window* self_win, int new_width, int new_height);
void terminal_on_input(char key);

#endif


