#include "kernel.h"
#include "IDT.c"
#include <stdint.h>

// kernel.c AUG 14


#define VGA_COLS 80 //x 
#define VGA_ROWS 25 // y 



extern void* isr_stub_table[];

void clear_screen(){
    unsigned int i;

    for (i = 0; i < VGA_COLS* VGA_ROWS; i++) {
        vga[i] = (0x07 << 8) | 0x20;  // writting the ascii symbol space to clear the screen (genius)
    }

}


void print_char(char character, unsigned char attribute, int posX, int posY) { 
   // vga[1] = (0x07 << 8) | 'i';

    vga[posY * VGA_COLS + posX] = (attribute << 8) | character;
}




void print_string(char* string, unsigned char attribute, int posX, int posY){  //works but you cant directly put a string as a agrument
    int i = 0;                                                                 //you need to put a variable. why? idk. it works so im not complaing

    while (string[i] != '\0') {
        print_char(string[i], attribute, posX + i, posY);
        i++;
    }

}



void kernel_main() {
   // clear_screen();


    char greeting[] = "Kernel was succesfully loaded";

    print_string(greeting, 0x07, 0, 2);


    char msgIDT[] = "Loading Interupt Data table";
    print_string(msgIDT, 0x07, 0, 3);


    idt_init();


   // int divzero = 1 / 0;

    char msgIDTSuccess[] = "Interupt Data table succesfuly loaded.";
    print_string(msgIDTSuccess, 0x07, 0, 4);


    





}



