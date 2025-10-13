#include "kernel.h"
#include "idt.c"
#include <stdbool.h>
#include <stdint.h>
#include "../libary/stdconsole.h"
#include "../libary/stdioaccess.h"
#include "isr.h"


// kernel.c AUG 14


bool is_pic = false;

#define PIC1		0x20		
#define PIC2		0xA0		
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)



extern void* isr_stub_table[];


bool loop = true;


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
    IDT_Initialize();
    ISR_Initialize();



    char msgIDT[] = "debug msg if here irq mask was cleared!";
    print_string(msgIDT, 0x07, 0, 10);

    asm volatile("sti");



    
}



