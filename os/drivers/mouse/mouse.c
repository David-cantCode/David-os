#include "stdint.h"
#include "../../libary/include/stdport.h"
#include "../../libary/include/stdconsole.h"
uint8_t mouse_cycle = 0;
uint8_t mouse_packet[3];

int mouse_x = 200;
int mouse_y = 200;
int mouse_left = 0;
int mouse_right = 0;

extern uint32_t screen_width;
extern uint32_t screen_height;

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