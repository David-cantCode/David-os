
#include <stdint.h>

#define SHITFT
extern volatile int key_down; // 0 f 1 true
extern volatile uint8_t scancode;
extern volatile int shift_down; // 0 f 1 t

#define SHIFT_KEY_P 0x2A
#define SHIFT_KEY_R 0xAA

//pressed n released


//assigns scancode to port 0x60 to get last key pressed
//added shift support 

void keyboard_callback(){
    key_down = 1;
    __asm__("in %%dx, %%al" : "=a" (scancode) : "d" (0x60)); //asgn scancode to inb on port 0x60
    

    if (scancode ==  SHIFT_KEY_P){
        shift_down = 1;
    }
    else if (scancode == SHIFT_KEY_R) {

        shift_down = 0;
    }

}