#include <stdint.h>



extern volatile unsigned int tick;  
extern uint32_t screen_width;
extern uint32_t screen_height;
extern uint32_t fb_addr;


#define SCREEN_W 1024
#define SCREEN_H 768 


extern uint8_t key_state[256];

static uint32_t back_buffer[SCREEN_W * SCREEN_H];
extern uint32_t pitch; 


static int ticks_per_frame = 1;
static int last_tick = 0;
static unsigned int target_fps = 60;
extern uint8_t font8x8[128][8];

uint8_t char_to_scancode(char c) {
    switch (c) {
        case 'w': return 0x11;
        case 's': return 0x1F;
        case 'a': return 0x1E;
        case 'd': return 0x20;
        case 'o': return 0x18;
        case 'l': return 0x26;
        case 'k': return 0x25;
        case ' ': return 0x39; 
        case 'q': return 0x10;
        case 'e': return 0x12;
        case 'p': return 0x19;
        default: return 0; 
    }
}


void set_fps(unsigned int fps) {
    if (fps == 0) fps = 1; // so no n / 0
    target_fps = fps;

    
    ticks_per_frame = 100 / target_fps;
    if (ticks_per_frame == 0) ticks_per_frame = 1;
}

int should_update() {
    if ((tick - last_tick) >= ticks_per_frame) {
        last_tick = tick;
        return 1;
    }
    return 0;
}



void screen_clear() {
    for (int i = 0; i < SCREEN_W * SCREEN_H; i++)
        back_buffer[i] = 0x00000000; 
}

void pixel(int x, int y, uint32_t color) {
    if (x < 0 || x >= SCREEN_W || y < 0 || y >= SCREEN_H) return;
    back_buffer[y * SCREEN_W + x] = color;

    
}

//copy backbuffer 2 front
void flip() {
    uint8_t* fb = (uint8_t*)fb_addr;
    for (int y = 0; y < SCREEN_H; y++) {
        for (int x = 0; x < SCREEN_W; x++) {
            uint32_t* pixel_addr = (uint32_t*)(fb + y * pitch + x * 4);
            *pixel_addr = back_buffer[y * SCREEN_W + x];
        }
    }
}


void draw_rect(int x, int y, int w, int h, uint32_t color) {
    for (int row = 0; row < h; row++) {
        for (int col = 0; col < w; col++) {
            pixel(x + col, y + row, color);
        }
    }
}

void draw_circle(int cx, int cy, int radius, uint32_t color) {
    int x = radius;
    int y = 0;
    int err = 0;

    while (x >= y) {
        pixel(cx + x, cy + y, color);
        pixel(cx + y, cy + x, color);
        pixel(cx - y, cy + x, color);
        pixel(cx - x, cy + y, color);
        pixel(cx - x, cy - y, color);
        pixel(cx - y, cy - x, color);
        pixel(cx + y, cy - x, color);
        pixel(cx + x, cy - y, color);

        y++;
        if (err <= 0) err += 2 * y + 1;
        if (err > 0) {
            x--;
            err -= 2 * x + 1;
        }
    }
}


int is_pressed(char imp) {
    uint8_t scancode = char_to_scancode(imp);
    if (scancode == 0) return 0;
    return key_state[scancode];
}


void d_char(int x, int y, char c, uint32_t color, int size) {
    if (c < 0 || c > 127) return;
    if (size < 1) size = 1;

    uint8_t* glyph = font8x8[(uint8_t)c];

    for (int row = 0; row < 8; row++) {
        uint8_t row_bits = glyph[row];
        for (int col = 0; col < 8; col++) {
            if (row_bits & (1 << col)) {
                //scale it
                for (int dy = 0; dy < size; dy++) {
                    for (int dx = 0; dx < size; dx++) {
                        pixel(x + col * size + dx, y + row * size + dy, color);
                    }
                }
            }
        }
    }
}
void draw_text(int x, int y, const char* str, uint32_t color, int size) {
    int cursor_x = x;
    while (*str) {
        d_char(cursor_x, y, *str, color, size);
        cursor_x += 8 * size; 
        str++;
    }
}