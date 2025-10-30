
#include <stdint.h>


#define KEY_DOWN_SCANCODE_LIMIT 57
#define BACKSPACE 0x0e
#define ENTER 0x1C

extern volatile int key_down; // 0 f 1 true
extern volatile uint8_t scancode;





//assigns scancode to port 0x60 to get last key pressed

void keyboard_callback(){
    key_down = 1;
    __asm__("in %%dx, %%al" : "=a" (scancode) : "d" (0x60)); //asgn scancode to inb on port 0x60
}