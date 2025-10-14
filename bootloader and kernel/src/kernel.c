#include "kernel.h"
#include "idt.c"
#include <stdbool.h>
#include <stdint.h>
#include "../libary/stdconsole.h"
#include "isr.h"


// kernel.c AUG 14


extern volatile int key_down;
//extern volatile char typed_letter;



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
    //clear_screen();
    __asm__ volatile ("cli");
    IDT_Initialize();
    ISR_Initialize();


    

    char msgIDT[] = "Kernel was loaded";
    print_string(msgIDT, 0x07, 0, 2);

  

    asm volatile("sti");
    uint8_t tmp = inb(0x60);
    if (key_down == 1){

        char p[] = "Typing";
        print_string(p, 0x07, 0, 5);
        // print_char(typed_letter, 0x07, 0, 1);
        key_down = 0;
    }
    
}

