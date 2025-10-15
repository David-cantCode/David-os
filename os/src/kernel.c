// kernel.c AUG 14


#include "cpu/idt.c"
#include "../libary/stdconsole.h"
#include "cpu/isr.h"
#include <stdint.h>






volatile int key_down;
volatile uint8_t scancode;
uint8_t input;

const char scancode_to_char[] = {
    '?', '?', '1', '2', '3', '4', '5',
    '6', '7', '8', '9', '0', '?', '?',
    '?', '?', 'Q', 'W', 'E', 'R', 'T',
    'Y', 'U', 'I', 'O', 'P', '?', '?',
    '?', '?', 'A', 'S', 'D', 'F', 'G',
    'H', 'J', 'K', 'L', '?', '?', '?',
    '?', '?', 'Z', 'X', 'C', 'V', 'B',
    'N', 'M', '?', '?', '?', '?', '?',
    '?', ' '
    };






//todo fix this stupid ahh function that gets the incorrect character
char scancode_to_ascii(uint8_t scancode) {
    //1–9 + null terminator
    if (scancode >= 0x02 && scancode <= 0x0A)
        return '1' + (scancode - 0x02);
    if (scancode == 0x0B) return '0';

    //Q–P
    if (scancode >= 0x10 && scancode <= 0x19)
        return 'Q' + (scancode - 0x10);

    //A–L
    if (scancode >= 0x1E && scancode <= 0x26)
        return 'A' + (scancode - 0x1E);

    //Z–M
    if (scancode >= 0x2C && scancode <= 0x32)
        return 'Z' + (scancode - 0x2C);

    if (scancode == 0x39) return ' ';   //space
   
   
   //ret unknown
    return 0; 
}


void kernel_main() {
    //**************************
    // *********INITIALLIZE*****
    // *************************
     __asm__ volatile ("cli");
    IDT_Initialize();
    ISR_Initialize();
    char msgIDT[] = "Kernel was loaded";
    print_string(msgIDT, 0x07, 0, 2);






    //*******************
    // *****MAIN*******
    // ***************** 
    asm volatile("sti");
    while (1) {
        // pause till interupt
        asm volatile("hlt");
        if (key_down == 1) {
            key_down = 0;


        char typing[] = "typing";
        print_string(typing, 0x07, 0, 3);

    input = scancode;

    char c = scancode_to_ascii(input);
    if (c) {
        print_char(c, 0x07, 0,5);
        
    }


        
    }
    
}
}
