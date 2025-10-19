// kernel.c AUG 14

#include "cpu/idt.c"
#include "../libary/include/stdconsole.h"
#include "cpu/isr.h"
#include "shell/shell.c"

volatile int key_down;


void kernel_main(){

    //*******************
    // *****MAIN*******
    // ***************** 
    asm volatile("sti");
    while (1) {
        // pause till interupt
        asm volatile("hlt");
        if (key_down == 1) {
            shell_main(scancode);
            key_down = 0;        
    }

    

    
    }
}



void kernel_ini() {
    //**************************
    // *********INITIALLIZE*****
    // *************************
     __asm__ volatile ("cli");
    char msgKernel[] = "Kernel was loaded";
    print_string(msgKernel, 0x07, 0, 1);
    
    IDT_Initialize();
    char msgIdt[] = "Interupt Descriptor Table was loaded";
    print_string(msgIdt, 0x07, 0, 2);


    ISR_Initialize();
    char msgIsr[] = "PIC was Remaped";
    print_string(msgIsr, 0x07, 0, 3);



    shell_ini();
    char msgShell[] = "Shell was Loaded";
    print_string(msgShell, 0x07, 0, 4);



    kernel_main();
}


