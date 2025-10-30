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
#define VBE_INFO_ADDR 0x400


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

    uint32_t* vbe = (uint32_t*)VBE_INFO_ADDR;

    fb_addr       = vbe[0]; // 0x0400
    screen_width  = vbe[1]; // 0x0404
    screen_height = vbe[2]; // 0x0408
    pitch         = vbe[3]; // 0x040C


    for (int x =0; x <10; x ++){
    put_pixel(x, 5, 0x0000FF00);
    }

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


