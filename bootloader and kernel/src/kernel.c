#include "kernel.h"
#include "idt.c"
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
    //**************************
    // *********INITIALLIZE*****
    // *************************

    __asm__ volatile ("cli");
    IDT_Initialize();
    ISR_Initialize();
    char msgIDT[] = "Kernel was loaded";
    print_string(msgIDT, 0x07, 0, 2);





    //*******************
    // *****LOOP*******
    // ***************** 
    asm volatile("sti");
    while (1) {
        // pause till interupt
        asm volatile("hlt");

        if (key_down == 1) {


            key_down = 0;
            char p[] = "Typing"; print_string(p, 0x07, 0, 5);  
        
        
        
        
        
        
        
        }

        
    }
    
}

