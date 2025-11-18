#include <stdint.h>
#include "../../libary/include/davidgl.h" 
#include "../../libary/include/fat16.h"
#include "../../libary/include/util.h"
#include "../../drivers/include/ata.h"
#include "d_edit.h"

//powered by david gl lol


extern uint8_t key_state[256];
extern volatile unsigned int tick;
extern uint32_t screen_width;
extern uint32_t screen_height;

static char textbuf[MAX_TEXT];
static int text_len = 0;

static int cursor_x = 0;
static int cursor_y = 0;
static int line_height = 16; // 8px font * size (2)
static int size = 2;

static char* file_name = "TEMP.TXT";

//commands:
//ctrl + s - save
//ctrl + q - quit



//todo 
// when saving files it just creates a new one
// add different dir save support 





static void editor_insert_char(char c) {
    if (text_len >= MAX_TEXT - 1) return;
    textbuf[text_len++] = c;
    textbuf[text_len] = '\0';

    if (c == '\n') {
        cursor_x = 0;
        cursor_y += line_height;
    } else {
        cursor_x += 8 * size; // * size
        if (cursor_x > screen_width - 20) {
            cursor_x = 0;
            cursor_y += line_height;
        }
    }
}


static void editor_backspace() {
    if (text_len <= 0) return;

    char last = textbuf[text_len - 1];
    text_len--;
    textbuf[text_len] = '\0';

    if (last == '\n') {
        cursor_x = 0;
        cursor_y = 0;

        for (int i = 0; i < text_len; i++) {
            if (textbuf[i] == '\n') {
                cursor_y += line_height;
                cursor_x = 0;
            } else {
                cursor_x += 8 * size;
                if (cursor_x > screen_width - 20) {
                    cursor_x = 0;
                    cursor_y += line_height;
                }
            }
        }
        return;
    }

    cursor_x -= 8 * 2;
    if (cursor_x < 0) cursor_x = 0;
}


static void d_edit_save() {
    char* buff;


    int err = fat_create_file_root(file_name,(const uint8_t*)textbuf,(uint32_t)text_len);

    if (err == 0) {

        s_printf(buff, "Saved %s", (uint32_t)file_name);
        draw_text(20, screen_height- 30, buff, 0xFFFFFF, 2);
    } else {
        draw_text(20, screen_height - 30, "Save FAILED", 0xFF0000, 2);
    }
    flip();
}

static int d_edit_open(const char* fname) {
    uint8_t fatname[11];
    make_fat11_name(fname, fatname);

    uint8_t dirbuf[SECTOR_SIZE];
    uint16_t starting_cluster = 0;
    uint32_t file_size = 0;
    int found = 0;

    //scan root dir sectors
    uint32_t sector = FIRST_ROOT_DIR_SECTOR;
    for (int s = 0; s < ROOT_DIR_SECTORS; s++, sector++) {
        read_sector(sector, dirbuf);
        for (int off = 0; off < SECTOR_SIZE; off += 32) {
            uint8_t first = dirbuf[off];
            if (first == 0x00) { //no more entries
                text_len = 0;
                textbuf[0] = '\0';
                return -1;
            }
            if (first == 0xE5) continue; //deleted
            if (memorycompare((const char*)fatname, (const char*)&dirbuf[off], 11) == 0) {
                starting_cluster = dirbuf[off + 26] | (dirbuf[off + 27] << 8);
                file_size =  dirbuf[off + 28] |(dirbuf[off + 29] << 8) |(dirbuf[off + 30] << 16) |(dirbuf[off + 31] << 24);
                found = 1;
                break;
            }

        }
        if (found) break;
    }


    //read file data
    text_len = 0;
    uint32_t remaining = file_size;
    uint16_t cluster = starting_cluster;
    uint8_t sec[SECTOR_SIZE];

    while (cluster >= 2 && cluster < 0xFFF8 && remaining > 0) {
        uint32_t lba = FIRST_DATA_SECTOR + (cluster - 2) * SECTORS_PER_CLUSTER;
        for (int si = 0; si < SECTORS_PER_CLUSTER && remaining > 0; si++) {
            read_sector(lba + si, sec);
            uint32_t tocopy = remaining > SECTOR_SIZE ? SECTOR_SIZE : remaining;
            for (uint32_t i = 0; i < tocopy; i++) {
                if (text_len >= MAX_TEXT - 1) { remaining = 0; break; }
                textbuf[text_len++] = (char)sec[i];
            }
            remaining -= tocopy;
        }
        if (remaining == 0) break;
        cluster = get_fat_entry(cluster);
    }

    textbuf[text_len] = '\0';
    return 0;
}

static void set_cur_pos() {
    cursor_x = 0;
    cursor_y = 0;

    for (int i = 0; i < text_len; i++) {
        char c = textbuf[i];
        if (c == '\n') {
            cursor_x = 0;
            cursor_y += line_height;
        } else {
            cursor_x += 8 * 2; // 8px font * size
            if (cursor_x > screen_width - 20) {
                cursor_x = 0;
                cursor_y += line_height;
            }
        }
    }
}


static int input_poll(){
    if (key_state[0x1D] && key_state[0x1F]) { //ctrl + s
            d_edit_save();}

    if (key_state[0X1D] && key_state[0x10]){ //ctrl + q
        return 0;
    }
    
    for (char c = 'a'; c <= 'z'; c++) {
        if (is_pressed(c)) {
            editor_insert_char(c);
            while (is_pressed(c)); }}


    if (is_pressed(' ')) {
            editor_insert_char(' ');
            while (is_pressed(' '));
    }

    if (is_pressed('\n')) {
        editor_insert_char('\n');
        while (is_pressed('\n'));   
    }

    if (is_pressed('\b')) {  
        editor_backspace();
        while (is_pressed('\b'));}


    return 1;
}

static void d_edit_draw(){
    draw_text(10, 10, textbuf, 0xFFFFFF, 2);
    draw_text(cursor_x , cursor_y + 5, "|", 0xFFFFFFFF, 3);

}

void d_edit_main() {
    int running = 1;

    while(running){
    if(should_update()){
       screen_clear();


    running = input_poll();
   
 

    d_edit_draw();

    flip();


    }}
}




void d_edit_innit(char *fname){
    text_len = 0;
    cursor_x = 0;
    cursor_y = 0;
    textbuf[0] = '\0';

    uint32_t last_frame = 0;


    file_name = fname;

    set_fps(120);

    d_edit_open(fname);
    set_cur_pos(); 

    d_edit_main();

}




