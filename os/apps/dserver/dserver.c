
#include <stdint.h>

#include "../../libary/include/davidgl.h"
#include "dserver.h"
#include "../terminal/terminal.h"
#include "../../libary/include/memory.h"
#include "../../libary/include/util.h"

extern uint32_t fb_addr;
extern uint32_t pitch;
extern uint32_t screen_width;
extern uint32_t screen_height;

volatile int mod_down;

#define MAX_WINDOWS 12

int window_count = 0;
extern volatile int shift_down;


static Window windows[MAX_WINDOWS];


int focused_window = 0;
static uint8_t prev_keys[256] = {0};


#define KEY_COUNT 256

extern uint8_t scancode;
extern uint8_t key_state[256];
extern int key_down;


void tile_windows() {
    if (window_count == 0) return;

    int gap = 5;  

    int cols = (window_count > 2) ? 2 : window_count;
    int rows = (window_count + 1) / 2;

    int total_gap_x = (cols + 1) * gap + 2;
    int total_gap_y = (rows + 1) * gap + 2 ;

    int win_w = ((screen_width - total_gap_x) / cols) / 1.6;
    int win_h = (screen_height - total_gap_y) / rows;

    int idx = 0;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (idx >= window_count) return;

            Window* w = &windows[idx++];

            w->x = gap + c * (win_w + gap);
            w->y = gap + r * (win_h + gap);
            w->width  = win_w;
            w->height = win_h;

        
        
            //call each programs on_resize
                w->program.on_resize(w, win_w, win_h);
            }
        }
    }




static const uint32_t colors[MAX_WINDOWS] = {
    0x00000000,  
    0x12F5F500, 
    0x4FF012F0,  
    0xF7F5F80F   
};

Window* create_terminal() {
    if (window_count >= MAX_WINDOWS) return 0;
    Window* win = &windows[window_count];
    
    win->color = colors[window_count % 4];

    win->program.type = PROGRAM_TERMINAL;
    win->program.on_update = terminal_update;
    win->program.on_input = terminal_on_input;
    win->program.on_resize = terminal_on_resize;


    win->buffer = (char*)memoryalloc(256);

    memoryset(win->buffer, '0', 256);
    win->buffer[0] = '0';

    win->lines_buf = (char*)memoryalloc(128*128);
    memoryset(win->lines_buf, 0, 128 * 128);

    window_count++;
    tile_windows();
    return win;
}


void kill_window() {
    if (window_count == 0) return;

    for (int i = focused_window; i < window_count - 1; i++) {
        windows[i] = windows[i + 1];
    }

    window_count--;

    if (focused_window >= window_count) {
        focused_window = (window_count > 0) ? window_count - 1 : 0;
    }

    tile_windows();
}


void draw_window(Window* win, int focused) {
    uint32_t border_color = focused ? 0xFFFFFFFF : 0xFF550000;


    uint8_t r = (win->color >> 16) & 0xFF;
    uint8_t g = (win->color >> 8) & 0xFF;
    uint8_t b = (win->color) & 0xFF;

    if (focused) {
        r = (r + 70 > 255) ? 255 : r + 70;
        g = (g + 70 > 255) ? 255 : g + 70;
        b = (b + 70 > 255) ? 255 : b + 70;
    }

    uint32_t fill_color = 0xFF000000 | (r << 16) | (g << 8) | b;



    int border_size = 4;

    for (int t = 0; t < border_size; t++) {
        for (int x = t; x < win->width - t; x++) {
            draw_pixel(win->x + x, win->y + t, border_color);
            draw_pixel(win->x + x, win->y + win->height - 1 - t, border_color); 
        }
        for (int y = t; y < win->height - t; y++) {
            draw_pixel(win->x + t, win->y + y, border_color); 
            draw_pixel(win->x + win->width - 1 - t, win->y + y, border_color); 
        }
    }
}

void render_windows() {
    for (int i = 0; i < window_count; i++) {
        Window* w = &windows[i];
        draw_window(w, (i == focused_window));

      
         w->program.on_update(w);
        
    }
}

int was_pressed(int key) {
    //doesnt allow keys to be held
    int pressed = is_pressed(key);
    int result = pressed && !prev_keys[key];
    prev_keys[key] = pressed;
    return result;
}


int event_check() {
    static int last_enter = 0;

    int enter_pressed = was_pressed('\n');
    int w = was_pressed('w');
    int a = was_pressed('a');
    int s = was_pressed('s');
    int d = was_pressed('d');
    int q = was_pressed('q');
    int x = was_pressed('p');

    if (mod_down) {
        if (enter_pressed && !last_enter) {
            create_terminal();
        }

        //fix this
        else if (w) focused_window = (focused_window == 0) ? window_count - 1 : focused_window - 2;
        else if (s) focused_window = (focused_window + 2) % window_count;
        else if (a && focused_window > 0) focused_window--;
        else if (d && focused_window < window_count - 1) focused_window++;


        else if (q){kill_window();}


        else if (x){return 0;} // quit dserver
         
    }

    last_enter = enter_pressed;


    return 1;
}

void input_handler() {
 
    if (key_down) {
        key_down = 0; 

        if (window_count > 0) {
            Window* w = &windows[focused_window];
            if (w->program.on_input) {
                w->program.on_input(w,scancode);
            }
        }
    }
}



void display_server_loop() {
    int running = 1;
    while (running == 1) {
        if (should_update()) {
            screen_clear();

            running = event_check();    
            input_handler();
            render_windows();
        
            flip();
        }
    }


}


void display_init() {

    display_server_loop();
}
