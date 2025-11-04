#include "../include/stdconsole.h"

#include <stdint.h>




uint32_t fb_addr = 0;
uint32_t pitch = 0;
uint32_t screen_width = 0;
uint32_t screen_height = 0;

extern uint8_t font8x8[128][8];
#define FONT_W 8
#define FONT_H 8



#define PIXEL_SCALE_X 1   
#define PIXEL_SCALE_Y 2   


#define CHAR_W (FONT_W * PIXEL_SCALE_X) 
#define CHAR_H (FONT_H * PIXEL_SCALE_Y) 

//*************************
//main shell draw methods***
//**************************

void draw_char(char c, int x, int y, uint32_t fg, uint32_t bg) {
    for (int row = 0; row < 8; row++) {
        uint8_t bits = font8x8[(int)c][row];
        for (int col = 0; col < 8; col++) {
            uint32_t color = (bits & (1 << col)) ? fg : bg;

            for (int sy = 0; sy < PIXEL_SCALE_Y; sy++) {
                for (int sx = 0; sx < PIXEL_SCALE_X; sx++) {
                    put_pixel(x + col * PIXEL_SCALE_X + sx,
                              y + row * PIXEL_SCALE_Y + sy,
                              color);
                }
            }
        }
    }
}
void put_pixel(int x, int y, uint32_t color) {
    uint8_t* fb = (uint8_t*)fb_addr;
    uint32_t* pixel = (uint32_t*)(fb + y * pitch + x * 4);
    *pixel = color;
}


//************************************ 
//*********TERMINAL DRAW METHODS***** 
//***********************************

void put_pixel_ctx(Window* win, int x, int y, uint32_t color) {
    // translate to screen coords
    int fx = win->x + x;
    int fy = win->y + y;

    // draw_pixel writes into back_buffer
    put_pixel(fx, fy, color);
}

void draw_char_ctx(Window* win, char c, int x, int y, uint32_t fg, uint32_t bg) {
    for (int row = 0; row < FONT_H; row++) {
        uint8_t bits = font8x8[(int)c][row];
        for (int col = 0; col < FONT_W; col++) {
            uint32_t color = (bits & (1 << col)) ? fg : bg;
            for (int sy = 0; sy < PIXEL_SCALE_Y; sy++) {
                for (int sx = 0; sx < PIXEL_SCALE_X; sx++) {
                    put_pixel_ctx(win,
                        x + col * PIXEL_SCALE_X + sx,
                        y + row * PIXEL_SCALE_Y + sy,
                        color);
                }
            }
        }
    }
}
void terminal_print(Terminal* terminal, const char* str) {
    int x = terminal->cursor_col * CHAR_W;
    int y = terminal->cursor_row * CHAR_H;
    for (int i = 0; str[i]; i++) {
        draw_char_ctx(&terminal->win, str[i], x, y, 0xFFFFFFFF, 0x00000000);
        x += CHAR_W;
    }
}






// compute c n r based on framebuffer size
 int fb_cols() {
    return (screen_width ? (screen_width / CHAR_W) : 80);
}
 int fb_rows() {
    return (screen_height ? (screen_height / CHAR_H) : 25);
}

static int cursor_col = 0;
static int cursor_row = 0;
int control_row = 0;

void control_line(int row) {
    control_row = row;
    if (control_row < 0) control_row = 0;
    if (control_row >= fb_rows()) control_row = fb_rows() - 1;
}

int get_offset(int col, int row) {
    return 2 * (row * fb_cols() + col);
}

void set_cursor(int offset) {
    int cell_index = offset / 2;
    int cols = fb_cols();
    cursor_row = cell_index / cols;
    cursor_col = cell_index % cols;
    if (cursor_row < control_row) cursor_row = control_row;
    if (cursor_row >= fb_rows()) cursor_row = fb_rows() - 1;
    if (cursor_col >= fb_cols()) cursor_col = fb_cols() - 1;
}

int get_cursor() {
    return 2 * (cursor_row * fb_cols() + cursor_col);
}


static void fb_memmove(uint8_t *dst, const uint8_t *src, unsigned int n) {
    if (dst == src || n == 0) return;
    if (dst < src) {
        for (unsigned int i = 0; i < n; ++i) dst[i] = src[i];
    } else {
        for (unsigned int i = n; i-- > 0;) dst[i] = src[i];
    }
}

void set_char_at_video_memory(char character, int offset) {
    int cell_index = offset / 2;
    int cols = fb_cols();
    int row = cell_index / cols;
    int col = cell_index % cols;

    uint32_t fg = 0xFFFFFFFF;
    uint32_t bg = 0x00000000;

    draw_char(character, col * CHAR_W, row * CHAR_H, fg, bg);

    cursor_col = col;
    cursor_row = row;
}

int scroll_screen(int memory_offset) {
    int cols = fb_cols();
    int rows = fb_rows();

    uint8_t *fb = (uint8_t*)(uintptr_t)fb_addr;
    int src_row_px = (control_row + 1) * CHAR_H;
    int dst_row_px = control_row * CHAR_H;
    unsigned int row_block_bytes = (unsigned int)(rows - control_row - 1) * CHAR_H * pitch;

    uint8_t *src = fb + (unsigned int)src_row_px * pitch;
    uint8_t *dst = fb + (unsigned int)dst_row_px * pitch;

    fb_memmove(dst, src, row_block_bytes);

    // Clear last row
    int last_row = rows - 1;
    for (int c = 0; c < cols; ++c) {
        draw_char(' ', c * CHAR_W, last_row * CHAR_H, 0x00000000, 0xFFFFFFFF);
    }

    return memory_offset - 2 * cols;
}

void print(char *string) {
    int memory_offset = get_cursor();
    int cols = fb_cols();
    int rows = fb_rows();
    int i = 0;

    while (string[i] != '\0') {
        if (string[i] == '\n') {
            int cur_row = memory_offset / (2 * cols);
            cur_row++;              
            if (cur_row >= rows) {    // scroll if bottom reached
                memory_offset = scroll_screen(memory_offset);
                cur_row = rows - 1;
            }
            memory_offset = get_offset(0, cur_row); 
            cursor_col = 0;
            cursor_row = cur_row;
        } else {
         
            set_char_at_video_memory(string[i], memory_offset);
            memory_offset += 2;
            cursor_row = memory_offset / (2 * cols);
            cursor_col = (memory_offset / 2) % cols;

            //scroll if bot
            if (cursor_row >= rows) {
                memory_offset = scroll_screen(memory_offset);
                cursor_row = rows - 1;
            }
        }
        i++;
    }

    set_cursor(memory_offset);
}



void print_backspace() {
    int newCursor = get_cursor() - 2;
    if (newCursor < get_offset(0, control_row)) newCursor = get_offset(0, control_row);
    set_char_at_video_memory(' ', newCursor);
    set_cursor(newCursor);
}

void clear_screen() {
    int cols = fb_cols();
    int rows = fb_rows();
    for (int r = control_row; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            draw_char(' ', c * CHAR_W, r * CHAR_H, 0x00000000u, 0x00000000u);
        }
    }
    set_cursor(get_offset(0, control_row));
}

void print_hex(uint8_t val) {
    const char *hex = "0123456789ABCDEF";
    char out[3];
    out[0] = hex[(val >> 4) & 0xF];
    out[1] = hex[val & 0xF];
    out[2] = '\0';
    print(out); 
}

void dump_sector(uint8_t *buf) {
    for (int i = 0; i < 512; i++) {
        print_hex(buf[i]);
        print(" ");
        if ((i + 1) % 16 == 0)
            print("\n");
    }
}


const char scancode_to_char[] = {
    '?', '?', '1', '2', '3', '4', '5',
    '6', '7', '8', '9', '0', '?', '?',
    '?', '?', 'q', 'w', 'e', 'r', 't',
    'y', 'u', 'i', 'o', 'p', '?', '?',
    '?', '?', 'a', 's', 'd', 'f', 'g',
    'h', 'j', 'k', 'l', '?', '?', '?',
    '?', '?', 'z', 'x', 'c', 'v', 'b',
    'n', 'm', '?', '.', '?', '?', '?',
    '?', ' '
};
