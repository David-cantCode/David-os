#include "kernel.h"

// kernel.c AUG 14

#define VGA_ADDRESS 0xB8000 //vga txt mode buffer 
#define VGA_COLS 80 //x 
#define VGA_ROWS 25 // y 


static unsigned short* const vga = (unsigned short*)VGA_ADDRESS;



void clear_screen(){
    unsigned int i;

    for (i = 0; i < VGA_COLS* VGA_ROWS; i++) {
        vga[i] = (0x07 << 8) | 0x20;  // writting the ascii symbol space to clear the screen (genius)
    }

}


void print_char(char character, unsigned char attribute, int posX, int posY) { //fix this
    vga[posY * VGA_COLS + posX] = (attribute << 8) | character;
}




void print_string(const char* string, unsigned char attribute, int posX, int posY){  //fix this
    int i = 0;
    while (string[i] != '\0') {
        print_char(string[i], attribute, posX + i, posY);
        i++;
    }

}



void kernel_main() {
    clear_screen();


   // print_string("Hello world", 0x07, 0, 0); // white on black WHY ISNT THIS WORKING 
    
    vga[0] = (0x07 << 8) | 'H';
    vga[1] = (0x07 << 8) | 'e';
    vga[2] = (0x07 << 8) | 'l';
    vga[3] = (0x07 << 8) | 'l';
    vga[4] = (0x07 << 8) | 'o';
    vga[5] = (0x07 << 8) | ' ';
    vga[6] = (0x07 << 8) | 'W';
    vga[7] = (0x07 << 8) | 'o';
    vga[8] = (0x07 << 8) | 'r';
    vga[9] = (0x07 << 8) | 'l';
    vga[10] =(0x07 << 8) | 'd';
}



