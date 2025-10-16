#include <stdint.h>
#include "fetch.c"

#define KEY_DOWN_SCANCODE_LIMIT 57
#define BACKSPACE 0x0e
#define ENTER 0x1C

char key_buffer[256];

volatile uint8_t scancode;



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

void execute_command(char *input) {
    if (compare_string(input, "CLEAR") == 0) {
        clear_screen();
    }
    else if (compare_string(input, "FETCH")==0) {
        fetch();
    
    }
    else if (compare_string(input, "IS AMOROUS A NIGGER?")==0){
        print("yes \n" );
    }




    
}
    



int backspace(char buffer[]) {
    int len = string_length(buffer);
    if (len > 0) {
        buffer[len - 1] = '\0';
        return 1;
    } else {
        return 0;
    }
}


void shell_main(uint8_t scancode){

    if (scancode > KEY_DOWN_SCANCODE_LIMIT) return;



    if (scancode == BACKSPACE) {
       if (backspace(key_buffer)) print_backspace();
    }

    else if (scancode == ENTER) {
        print("\n");
        execute_command(key_buffer);
        key_buffer[0] = '\0';
        print("> ");
    }

    else{
        char letter = scancode_to_char[(int) scancode];
        append(key_buffer, letter);
        char str[2] = {letter, '\0'};
        print(str);
    }

}

void shell_ini(){
    print("> ");
}