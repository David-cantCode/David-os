#include "kernel.h"
#include "idt.c"
#include <stdbool.h>
#include <stdint.h>
#include "../libary/stdconsole.h"
#include "../drivers/pic.h"
#include "../libary/stdioaccess.h"


// kernel.c AUG 14


bool is_pic = false;

#define PIC1		0x20		
#define PIC2		0xA0		
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)



extern void* isr_stub_table[];

void clear_screen(){
    unsigned int i;

    for (i = 0; i < VGA_COLS* VGA_ROWS; i++) {
        vga[i] = (0x07 << 8) | 0x20;  // writting the ascii symbol space to clear the screen (genius)
    }

}

void IRQ_set_mask(uint8_t IRQline) {
    uint16_t port;
    uint8_t value;

    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = inb(port) | (1 << IRQline);
    outb(port, value);        




}




void kernel_main() {
   // clear_screen();
    __asm__ volatile ("cli");



    char greeting[] = "Kernel was entered.";

    print_string(greeting, 0x07, 0, 2);


    char load_idt[] = "Loading IDT";
    print_string(load_idt, 0x07, 0, 3);


    idt_init();


    char idt_sucess[] = "IDT was succesfully loaded";
    print_string(idt_sucess, 0x07, 0, 4);





    PIC_remap(0x20, 0x28);







    
    
    
    
    char msgpicSuccess[] = "Programmable Interrupt Controller successfuly loaded.";
    print_string(msgpicSuccess, 0x07, 0, 7);


    __asm__ volatile("cli");
    IRQ_clear_mask(1);

    char msgIDT[] = "debug msg if here irq mask was cleared!";
    print_string(msgIDT, 0x07, 0, 10);



    asm volatile("sti"); 



}



