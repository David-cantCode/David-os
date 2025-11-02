
#include <stdint.h>

#define KEY_COUNT 256
volatile uint8_t key_state[KEY_COUNT];

extern volatile int key_down;
extern volatile uint8_t scancode;
extern volatile int shift_down;

#define SHIFT_KEY_P 0x2A
#define SHIFT_KEY_R 0xAA

void keyboard_callback() {
    uint8_t code;
    __asm__("in %%dx, %%al" : "=a" (code) : "d" (0x60));

    scancode = code;  

    //key r
    if (code & 0x80) {
        uint8_t released = code & 0x7F;
        key_state[released] = 0;

        if (released == SHIFT_KEY_P) shift_down = 0;

       
        //key p
        int any_pressed = 0;
        for (int i = 0; i < KEY_COUNT; i++) {
            if (key_state[i]) { any_pressed = 1; break; }
        }
        key_down = any_pressed;
        return;
    }

    
    key_state[code] = 1;
    key_down = 1;

    if (code == SHIFT_KEY_P) shift_down = 1;
}