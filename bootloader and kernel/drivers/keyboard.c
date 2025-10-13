
//#include "../libary/stdconsole.h"
#include "../libary/stdioaccess.h"
#include <stdbool.h>


uint8_t scancode;


extern bool is_picc;



volatile uint8_t kbd_buf[256];
volatile uint32_t kbd_head = 0;





void keyboard_callback(void) {



 
}


