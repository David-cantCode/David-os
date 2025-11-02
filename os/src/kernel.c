// kernel.c AUG 14


#include "cpu/idt.c"
#include "../libary/include/stdconsole.h"
#include "cpu/isr.h"
#include "shell/shell.c"

volatile int key_down;


extern uint32_t fb_addr;
extern uint32_t pitch;
extern uint32_t screen_width;
extern uint32_t screen_height;
#define VBE_ADDR 0x400


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

    uint32_t* vbe = (uint32_t*)VBE_ADDR;

    fb_addr       = vbe[0]; // 0x0400
    screen_width  = vbe[1]; // 0x0404
    screen_height = vbe[2]; // 0x0408
    pitch         =  2560; //pitch ≈ width * (bits_per_pixel / 8)




    //char msgKernel[] = "Kernel was loaded";

    print("kernel was loaded \n");
    
    IDT_Initialize();
    print("Interupt Descriptor Table was loaded\n");


    ISR_Initialize();
    print("PIC was Remaped \n");


    print("Shell was Loaded \n");
    print("\n");

    shell_ini();

 



    kernel_main();
}

