#include "../include/stdconsole.h"

#include <stdint.h>




uint32_t fb_addr = 0;
uint32_t pitch = 0;
uint32_t screen_width = 0;
uint32_t screen_height = 0;

void print_char(char character, unsigned char attribute, int posX, int posY) { 
   
                                
    vga[posY * VGA_COLS + posX] = (attribute << 8) | character;
}


                                //for ground and background attribute 
void print_string(char* string, unsigned char attribute, int posX, int posY){  //works but you cant but a strign as a agr. please use a char array
    int i = 0;                                                                 

    while (string[i] != '\0') {
        print_char(string[i], attribute, posX + i, posY);
        i++;
    }

}

//convert byte to hex string 

static char* byte_to_hex(uint8_t b, char* buffer) {
    const char* hex_chars = "0123456789ABCDEF";
    buffer[0] = hex_chars[(b >> 4) & 0x0F];
    buffer[1] = hex_chars[b & 0x0F];
    buffer[2] = '\0';
    return buffer;

}




//vbe shit

void draw_char( int x, int y, uint32_t fg, uint32_t bg) {
    // Hardcode the letter 'A' bitmap for now
    uint8_t glyph[8] = {
    0b00011000,
    0b00100100,
    0b01000010,
    0b01111110,
    0b01000010,
    0b01000010,
    0b01000010,
    0b00000000
    };

    for (int row = 0; row < 8; row++) {
        uint8_t bits = glyph[row];   
        for (int col = 0; col < 8; col++) {
        
            uint32_t color = (bits & (1 << (7 - col))) ? fg : bg;

            put_pixel(x + col, y + row,color);
        }
    }
}


void put_pixel(int x, int y, uint32_t color) {
    uint8_t* fb = (uint8_t*)fb_addr;
    uint32_t* pixel = (uint32_t*)(fb + y * pitch + x * 4);
    *pixel = color;
}



