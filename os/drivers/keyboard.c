
#include <stdint.h>



extern volatile int key_down; // 0 f 1 true
extern volatile uint8_t scancode;




void keyboard_callback(){
    key_down = 1;
    __asm__("in %%dx, %%al" : "=a" (scancode) : "d" (0x60)); //asgn scancode to inb on port 0x60
}