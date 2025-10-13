#include "idt.h"
#include "../libary/stdioaccess.h"

IDT_gate g_IDT[256];

IDT_register g_IDTDescriptor;


void IDT_SetGate(int interrupt, unsigned long base)
{
    g_IDT[interrupt].offset_lower = (base) & 0xFFFF;
    g_IDT[interrupt].segment_selector = 0x08;
    g_IDT[interrupt].always0 = 0;
    g_IDT[interrupt].flags = 0x8E;
    g_IDT[interrupt].offset_higher = (base >> 16) & 0xFFFF;
}

void IDT_Initialize()
{   
    g_IDTDescriptor.base = (unsigned long) &g_IDT;
    g_IDTDescriptor.limit = sizeof(g_IDT) - 1;
    IDT_load(&g_IDTDescriptor);

     outb(0x20, 0x11);
     outb(0xA0, 0x11);

    // ICW2
     outb(0x21, 0x20);
     outb(0xA1, 0x28);

    // ICW3
     outb(0x21, 0x04);
    outb(0xA1, 0x02);

    // ICW4
     outb(0x21, 0x01);
     outb(0xA1, 0x01);

    // OCW1
     outb(0x21, 0xFC); // 1111 1100 -> unmask IRQ0 (timer) and unmask IRQ1 (keyboard)
     outb(0xA1, 0xFF);
}