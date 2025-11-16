#include <stdint.h>
#include "../../libary/include/davidgl.h"
#include "../../libary/include/fat16.h"
#include "../../libary/include/util.h"
#include "d_edit.h"

extern uint8_t key_state[256];
extern volatile unsigned int tick;
extern uint32_t screen_width;
extern uint32_t screen_height;

static char textbuf[MAX_TEXT];
static int text_len = 0;

static int cursor_x = 0;
static int cursor_y = 0;
static int line_height = 16; // 8px font * size (2)


static char* file_name = "TEMP.TXT";

//commands:
//ctrl + s - save
//ctrl + q - quit



static void editor_insert_char(char c) {
    if (text_len >= MAX_TEXT - 1) return;
    textbuf[text_len++] = c;
    textbuf[text_len] = '\0';

    if (c == '\n') {
        cursor_x = 0;
        cursor_y += line_height;
    } else {
        cursor_x += 8 * 2; // * size
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
                cursor_x += 8 * 2;
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
    draw_text(cursor_x , cursor_y + 5, "|", 0x00FF00, 3);

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

    d_edit_main();

}




