#include "kernel.h"
#include "cpu/idt.c"

#include "../libary/stdconsole.h"
#include "cpu/isr.h"


// kernel.c AUG 14


volatile int key_down;
volatile uint8_t scancode;


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


char input[] = {'h', 'i'};

int compare_string(char s1[], char s2[]) {
    int i;
    for (i = 0; s1[i] == s2[i]; i++) {
        if (s1[i] == '\0') return 0;
    }
    return s1[i] - s2[i];
}


void execute_command(char *input) {

   if (compare_string(input, "CLEAR") == 0) {
        clear_screen();
    }

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
    clear_screen();
    while (1) {
        // pause till interupt
        asm volatile("hlt");
        if (key_down == 1) {
            key_down = 0;

            if (scancode < 57) {
                input[0] = scancode_to_char[scancode];
                print_string(input, 0x07, 0, 3);
            } 

            char p[] = "Typing";
             print_string(p, 0x07, 0, 5);  
        
            


        }

        
    }
    
}

