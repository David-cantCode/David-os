
#include <stdint.h>


#define IDT_FLAG_PRESENT 0x80
#define IDT_FLAG_INT32   0x0E
uint8_t flags = IDT_FLAG_PRESENT | IDT_FLAG_INT32;

#define VGA_ADDRESS 0xB8000 //vga txt mode buffer 

static unsigned short* const vga = (unsigned short*)VGA_ADDRESS;

typedef struct {
	uint16_t    isr_low;      // The lower 16 bits of the ISR's address
	uint16_t    kernel_cs ;    // The GDT segment selector that the CPU will load into CS before calling the ISR
	uint8_t     reserved;     // Set to zero
	uint8_t     attributes;   // Type and attributes; see the IDT page
	uint16_t    isr_high;     // The higher 16 bits of the ISR's address



} __attribute__((packed)) idt_entry_t;

typedef struct {
	uint16_t	limit;
	uint32_t	base;
} __attribute__((packed)) idtr_t;


static idtr_t idtr;
__attribute__((aligned(0x10))) 
static idt_entry_t idt[256]; //IDT entries


    
__attribute__((noreturn))
void exception_handler(void);
void exception_handler() {
    vga[0] = (0x4F << 8) | 'E'; //debug letters doesnt show
    vga[1] = (0x4F << 8) | 'X';
    vga[2] = (0x4F << 8) | 'C';
    vga[3] = (0x4F << 8) | '!';
    __asm__ volatile ("cli");  // disable interrupts
    __asm__ volatile ("hlt");  // halt CPU

}



void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags);
void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
    idt_entry_t* descriptor = &idt[vector];

    descriptor->isr_low        = (uint32_t)isr & 0xFFFF;
    descriptor->kernel_cs      = 0x08; //code seg from boot.asm
    descriptor->attributes     = flags;
    descriptor->isr_high       = (uint32_t)isr >> 16;
    descriptor->reserved       = 0;


}


void load_idt() {
    vga[20] = (0x07 << 8) | 'X';



    struct {
        uint16_t limit;
        uint32_t base;
    } __attribute__((packed)) idtr;

    idtr.limit = sizeof(idt_entry_t) * 256 - 1;
    idtr.base  = (uint32_t)&idt;
    __asm__ volatile("lidt %0" : : "m"(idtr));
}
