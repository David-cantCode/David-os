#include <stdint.h>
#include "../../apps/fetch/fetch.c"
#include "../../apps/help/help.c"

#include "../../drivers/include/ata.h"

#define KEY_DOWN_SCANCODE_LIMIT 57
#define BACKSPACE 0x0e
#define ENTER 0x1C

char key_buffer[256];

volatile uint8_t scancode;


uint8_t buff[512];


#define TEST_SECTOR 1
#define SECTOR_SIZE 512
uint8_t write_buf[SECTOR_SIZE];

void fill_test_pattern(uint8_t* buf, uint8_t pattern) {
    for (int i = 0; i < SECTOR_SIZE; i++) {
        buf[i] = pattern;
    }
}

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
    else if (compare_string(input, "HELP")==0){
        show_commands();
    }
    else if(compare_string(input, "READ BOOT") == 0){
        read_sector(0, buff);
        dump_sector(buff);
    }

    else if (compare_string(input, "WRITE TEST") == 0){
    fill_test_pattern(write_buf, 0xAB);  
    write_sector(TEST_SECTOR, write_buf);

    print("Sector 1 was overwritten\n");
    }
    else if (compare_string(input, "READ TEST") ==0){

        read_sector(TEST_SECTOR, buff);
        dump_sector(buff);
    }




    else{
        print("unknown command '");
        print(input);
        print("' \ntype 'help' for info\n \n");
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