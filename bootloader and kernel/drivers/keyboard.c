#include "port.h"
#include "../libary/stdconsole.h"

#define KEY_DOWN_SCANCODE_LIMIT 57
#define BACKSPACE 0x0e
#define ENTER 0x1C

char key_buffer[256];

int compare_string(char s1[], char s2[]) {
    int i;
    for (i = 0; s1[i] == s2[i]; i++) {
        if (s1[i] == '\0') return 0;
    }
    return s1[i] - s2[i];
}


void execute_command(char *input) {

   if (compare_string(input, "CLEAR") == 0) {
        clear_screen();
    }

}




void keyboard_callback(){

    unsigned char scancode = port_byte_in(0x60);
    if (scancode > KEY_DOWN_SCANCODE_LIMIT) return;

    const char scancode_to_char[] = {
    '?', '?', '1', '2', '3', '4', '5',
    '6', '7', '8', '9', '0', '?', '?',
    '?', '?', 'Q', 'W', 'E', 'R', 'T',
    'Y', 'U', 'I', 'O', 'P', '?', '?',
    '?', '?', 'A', 'S', 'D', 'F', 'G',
    'H', 'J', 'K', 'L', '?', '?', '?',
    '?', '?', 'Z', 'X', 'C', 'V', 'B',
    'N', 'M', '?', '?', '?', '?', '?',
    '?', ' '
    };

  

    if (scancode == ENTER) {
        //print("\n");
        execute_command("CLEAR");
        key_buffer[0] = '\0';
    }
    else{
        char letter = scancode_to_char[(int) scancode];
        append(key_buffer, letter);
        char str[2] = {letter, '\0'};
        print_string(str, 0x07, 0, 10);
    }
     
}