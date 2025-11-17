#include "stdint.h"
#include "../../libary/include/stdport.h"
#include "../../libary/include/stdconsole.h"
uint8_t mouse_cycle = 0;
uint8_t mouse_packet[3];


void mouse_init() {
    outb(0x64, 0xA8); //enable auxiliary device
    outb(0x60, 0xF4); //enable data reporting

    print("mouse was initialized\n");
    
}


void mouse_callback() {
    uint8_t data = inb(0x60);

    if (mouse_cycle == 0) mouse_packet[0] = data;
    else if (mouse_cycle == 1) mouse_packet[1] = data;
    else if (mouse_cycle == 2) {
        mouse_packet[2] = data;
        mouse_cycle = 0;

        int x = (int8_t)mouse_packet[1];
        int y = (int8_t)mouse_packet[2];
        int left = mouse_packet[0] & 0x1;
        int right = mouse_packet[0] & 0x2;

        
    } else mouse_cycle++;





    outb(0x20, 0x20); 
    outb(0xA0, 0x20); 
}
