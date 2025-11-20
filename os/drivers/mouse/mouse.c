#include "stdint.h"
#include "../../libary/include/stdport.h"
#include "../../libary/include/stdconsole.h"
#include "util.h"
uint8_t mouse_cycle = 0;
uint8_t mouse_packet[3];

int mouse_x = 200;
int mouse_y = 200;
int mouse_left = 0;
int mouse_right = 0;

extern uint32_t screen_width;
extern uint32_t screen_height;

//doesnt work 
// i tried doing qemu-system-i386 -hda bin/davidos.iso -m 4G -machine pc -device i8042
//still nothing ill check this out later
void mouse_init() {
    outb(0x64, 0xA8); //enable auxiliary device
    outb(0x60, 0xF4); //enable data reporting
    outb(0x64, 0xD4);  

    uint8_t status = inb(0x64);
    char buf[64];
    s_printf(buf, "PS2 status after A8 = %x\n", status);
    print(buf);


    print("mouse was initialized\n");
    
}

void mouse_callback() {
    uint8_t data = inb(0x60);
    print("mouse be going brrrr");

    if (mouse_cycle == 0) mouse_packet[0] = data;
    else if (mouse_cycle == 1) mouse_packet[1] = data;
    else if (mouse_cycle == 2) {


        mouse_packet[2] = data;
        mouse_cycle = 0;

        //get mouse imp and mov
        int x = (int8_t)mouse_packet[1];
        int y = (int8_t)mouse_packet[2];
        mouse_left  = mouse_packet[0] & 0x1;
        mouse_right = mouse_packet[0] & 0x2;
        mouse_x += x;
        mouse_y -= y;    

      
        //clamp
        if (mouse_x < 0) mouse_x = 0;
        if (mouse_y < 0) mouse_y = 0;
        if (mouse_x >= screen_width) mouse_x = screen_width - 1;
        if (mouse_y >= screen_height) mouse_y = screen_height - 1;

    } else mouse_cycle++;

    outb(0x20, 0x20); 
    outb(0xA0, 0x20);
}