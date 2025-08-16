#include "stdconsole.h"
#include <stdint.h>



void print_char(char character, unsigned char attribute, int posX, int posY) { 
   
                                
    vga[posY * VGA_COLS + posX] = (attribute << 8) | character;
}


                                //for ground and background attribute 
void print_string(char* string, unsigned char attribute, int posX, int posY){  //works but you cant but a strign as a agr. please use print_string(msg[] )
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

