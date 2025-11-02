
#include <stdint.h>

#include "../../libary/include/davidgl.h"
#include "display.h"

extern uint32_t fb_addr;
extern uint32_t pitch;
extern uint32_t screen_width;
extern uint32_t screen_height;

volatile int mod_down;

#define MAX_WINDOWS 4

int window_count;

typedef struct {
    int x, y;               
    int width, height;
    
} Window;

static Window windows[MAX_WINDOWS];



Window* create_window(int x, int y, int w, int h) {
    if (window_count >= MAX_WINDOWS) return 0;
    Window* win = &windows[window_count++];
    win->x = x;
    win->y = y;
    win->width = w;
    win->height = h;
    return win;
}

void draw_window(Window* win) {
    for (int y = 0; y < win->height; y++)
        for (int x = 0; x < win->width; x++) {
            uint32_t color = 0xFFFFFFFF;
            if (color) draw_pixel(win->x + x, win->y + y, color);
        }
}

void render_windows() {
    for (int i = 0; i < window_count; i++) {
        draw_window(&windows[i]);
    }
}

void event_check(){
    if (mod_down == 1 && is_pressed('\n')){
        create_window(420, 50, 200, 500);
    }


}


void display_server_loop() {
    while (1) {
        if (should_update()) {
        screen_clear();

        event_check();
        render_windows();

           
            flip(); 

        }
    }
}


void display_main() {
  
    Window* win1 = create_window(50, 50, 200, 500);


    display_server_loop();

}

