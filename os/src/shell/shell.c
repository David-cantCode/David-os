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




<<<<<<< Updated upstream
=======
    else if(compare_string(input, "READ BOOT") == 0){
        read_sector(0, sector_buff);
        dump_sector(sector_buff);
    }

    else if (compare_string(input, "WRITE SECTOR") == 0){
    fill_test_pattern(write_buf, 0xAB);  
    write_sector(TEST_SECTOR, write_buf);

    print("Sector 1 was overwritten\n");
    }
    else if (compare_string(input, "READ SECTOR") ==0){

        read_sector(TEST_SECTOR, sector_buff);
        dump_sector(sector_buff);
    }


    else if (compare_string(input, "READ TEST TEXT")==0){
        uint32_t test_file_lba = FIRST_DATA_SECTOR + (2 - 2) * SECTORS_PER_CLUSTER;
        read_sector(test_file_lba, sector_buff);
        dump_sector(sector_buff);
    }

    else if (starts_with(input, "MKDIR ")) {
    char* name = input + 6;
    name = conv_fat_name(name);

    create_directory(name, cur_dir_cluster, cur_dir_cluster == 0);
    print("Created directory: ");
    print(name);
    print("\n \n");
    }
    
    else if (starts_with(input, "CD ")) {
        char* name = input + 3;
        name = conv_fat_name(name);

        uint16_t new_cluster = find_directory_cluster(name, cur_dir_cluster);

        if (new_cluster != 0xFFFF) {
            cur_dir_cluster = new_cluster;
            memorycpy(cur_dir_name, name, 8); //set it for the alias


            print("\n");}
        else {
            print("Directory '");
            print(input + 3); 
            print("' not found.\n");}
                
    }

    else if (compare_string(input, "LS") == 0) {
    list_directory(cur_dir_cluster);
    print("\n");
    }


    else{
        print("unknown command '");
        print(input);
        print("' \ntype 'help' for info\n \n");
    }
>>>>>>> Stashed changes
    
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