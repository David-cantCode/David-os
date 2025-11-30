#include <stdint.h>
#include "../include/davidgl.h"
#include "../include/util.h"

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
extern uint8_t font_8bit[128][8];

uint8_t char_to_scancode(char c) {
    switch (c) {
        case '1': return 0x02;
        case '2': return 0x03;
        case '3': return 0x04;
        case '4': return 0x05;
        case '5': return 0x06;
        case '6': return 0x07;
        case '7': return 0x08;
        case '8': return 0x09;
        case '9': return 0x0A;
        case '0': return 0x0B;

        case 'q': return 0x10;
        case 'w': return 0x11;
        case 'e': return 0x12;
        case 'r': return 0x13;
        case 't': return 0x14;
        case 'y': return 0x15;
        case 'u': return 0x16;
        case 'i': return 0x17;
        case 'o': return 0x18;
        case 'p': return 0x19;

        case 'a': return 0x1E;
        case 's': return 0x1F;
        case 'd': return 0x20;
        case 'f': return 0x21;
        case 'g': return 0x22;
        case 'h': return 0x23;
        case 'j': return 0x24;
        case 'k': return 0x25;
        case 'l': return 0x26;

        case 'z': return 0x2C;
        case 'x': return 0x2D;
        case 'c': return 0x2E;
        case 'v': return 0x2F;
        case 'b': return 0x30;
        case 'n': return 0x31;
        case 'm': return 0x32;

        case ' ': return 0x39;
        case '\n': return 0x1C;
        case '\b': return 0x0E;
        
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
    //does 2 pixles at a time instead
    uint64_t* ptr = (uint64_t*)back_buffer;
    int count = (SCREEN_W * SCREEN_H) / 2;

    while(count--) *ptr++ = 0;
}


void draw_pixel(int x, int y, uint32_t color) {
    if ((unsigned)x >= SCREEN_W || (unsigned)y >= SCREEN_H) return;
    back_buffer[y * SCREEN_W + x] = color;
}


//copy backbuffer 2 front
void flip() {
    uint8_t* fb = (uint8_t*)fb_addr;
    int row_bytes = SCREEN_W * 4;


    if (pitch == row_bytes) {
        memorycpy(fb, back_buffer, SCREEN_W * SCREEN_H * 4);
    } else {

        for (int y = 0; y < SCREEN_H; y++) {
            memorycpy(fb + y * pitch, &back_buffer[y * SCREEN_W], row_bytes);
        }
    }
}


void draw_rect(int x, int y, int w, int h, uint32_t color) {
    for (int row = 0; row < h; row++) {
        uint32_t* dst = &back_buffer[(y + row) * SCREEN_W + x];
        for (int col = 0; col < w; col++) {
            dst[col] = color;
        }
    }
}
void draw_circle(int cx, int cy, int radius, uint32_t color) {
    int x = radius;
    int y = 0;
    int err = 0;

    while (x >= y) {
        draw_pixel(cx + x, cy + y, color);
        draw_pixel(cx + y, cy + x, color);
        draw_pixel(cx - y, cy + x, color);
        draw_pixel(cx - x, cy + y, color);
        draw_pixel(cx - x, cy - y, color);
        draw_pixel(cx - y, cy - x, color);
        draw_pixel(cx + y, cy - x, color);
        draw_pixel(cx + x, cy - y, color);

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

    uint8_t* glyph = font_8bit[(uint8_t)c];

    for (int row = 0; row < 8; row++) {
        uint8_t row_bits = glyph[row];
        for (int col = 0; col < 8; col++) {
            if (row_bits & (1 << col)) {
                //scale it
                for (int dy = 0; dy < size; dy++) {
                    for (int dx = 0; dx < size; dx++) {
                        draw_pixel(x + col * size + dx, y + row * size + dy, color);
                    }
                }
            }
        }
    }
}
void draw_text(int x, int y, const char* str, uint32_t color, int size) {
    int cursor_x = x;
    int line_height = 8 * size;

    while (*str) {

        if (*str == '\n') {
            y += line_height;   
            cursor_x = x;       
            str++;
            continue;
        }

        d_char(cursor_x, y, *str, color, size);
        cursor_x += 8 * size; 
        str++;
    }
}

void draw_line(int x0, int y0, int x1, int y1, uint32_t color) {
    int dx = absolute(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -absolute(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;
    
    while (1) {
        draw_pixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}



void draw_sprite(int pos_x, int pos_y, Sprite sprite, float scale) {
    int dst_w = (int)(sprite.width * scale);
    int dst_h = (int)(sprite.height * scale);
    

    if (dst_w <= 0 || dst_h <= 0) return;

    //clip check
    int x0 = pos_x;
    int y0 = pos_y;
    int x1 = x0 + dst_w;
    int y1 = y0 + dst_h;

    if (x0 < 0) x0 = 0;
    if (y0 < 0) y0 = 0;
    if (x1 > SCREEN_W) x1 = SCREEN_W;
    if (y1 > SCREEN_H) y1 = SCREEN_H;

    //if completely offscreen ret
    if (x0 >= x1 || y0 >= y1) return;

    uint32_t step_x = (sprite.width << 16) / dst_w;
    uint32_t step_y = (sprite.height << 16) / dst_h;

    uint32_t u_start = (x0 - pos_x) * step_x;
    uint32_t v = (y0 - pos_y) * step_y;


    for (int y = y0; y < y1; y++) {

        int src_y = v >> 16;
        
        v += step_y;

      
        uint32_t* sprite_row = &sprite.pixles[src_y * sprite.width];

        uint32_t* screen_row = &back_buffer[y * SCREEN_W];

        uint32_t u = u_start;

        for (int x = x0; x < x1; x++) {
            int src_x = u >> 16;
            u += step_x;
            uint32_t color = sprite_row[src_x];

            if (color != 0x000000FF) {
                screen_row[x] = color;
            }
        }
    }
}