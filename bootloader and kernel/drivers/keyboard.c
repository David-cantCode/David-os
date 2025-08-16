//#include <stdint.h>
//#include "../libary/stdioaccess.h"
#include "../libary/stdconsole.h"
#include "pic.h"

static char* byte_to_hex(uint8_t b, char* buffer) {
    const char* hex_chars = "0123456789ABCDEF";
    buffer[0] = hex_chars[(b >> 4) & 0x0F];
    buffer[1] = hex_chars[b & 0x0F];
    buffer[2] = '\0';
    return buffer;
}




void keyboard_callback(void) {
    uint8_t scancode = inb(0x60);

    char hex_buf[3];
    byte_to_hex(scancode, hex_buf);

    char msg[32] = "Key pressed: ";


    msg[13] = hex_buf[0];
    msg[14] = hex_buf[1];
    msg[15] = '\0';

    print_string(msg, 0x07, 0, 6);
}