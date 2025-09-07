#include "kernel.h"
#include "idt.c"
#include <stdint.h>
#include "../libary/stdconsole.h"
#include "../drivers/pic.h"

// kernel.c AUG 14





extern void* isr_stub_table[];

void clear_screen(){
    unsigned int i;

    for (i = 0; i < VGA_COLS* VGA_ROWS; i++) {
        vga[i] = (0x07 << 8) | 0x20;  // writting the ascii symbol space to clear the screen (genius)
    }

}




void kernel_main() {
   // clear_screen();
    __asm__ volatile ("cli");

    //char greeting[] = "Kernel was succesfully loaded";

    //print_string(greeting, 0x07, 0, 2);


   // char msgIDT[] = "Loading Interupt Data table";
    //print_string(msgIDT, 0x07, 0, 3);


    idt_init();


    char msgIDTSuccess[] = "Interupt Data table succesfuly loaded.";
    print_string(msgIDTSuccess, 0x07, 0, 4);


    //char msgpic[] = "Loading The Programmable Interrupt Controller";
    //print_string(msgpic, 0x07, 0, 5);
    
 


    PIC_remap(0x8, 0x70);




    //IRQ_set_mask(0xFF);      //kills the stack never un comment keeping this to remind me of my 4  hours of debugging  (no joke)
    

    //next set the keyboard stud




    char greeting[] = "Kernel was succesfully loaded";
    print_string(greeting, 0x07, 0, 2);



    
    
    
    
    char msgpicSuccess[] = "Programmable Interrupt Controller successfuly loaded.";
    print_string(msgpicSuccess, 0x07, 0, 7);


    






}



