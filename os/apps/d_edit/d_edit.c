#include <stdint.h>
#include "../../libary/include/davidgl.h"
#include "../../libary/include/fat16.h"
#include "d_edit.h"
// from your GL code:
extern uint8_t key_state[256];


extern volatile unsigned int tick;


extern uint32_t screen_width;
extern uint32_t screen_height;

// ============================================================================
// TEXT EDITOR STATE
// ============================================================================
static char textbuf[MAX_TEXT];
static int text_len = 0;

static int cursor_x = 0;
static int cursor_y = 0;
static int line_height = 16; // 8px font * size 2

// ============================================================================
// APPEND CHARACTER
// ============================================================================
static void editor_insert_char(char c) {
    if (text_len >= MAX_TEXT - 1) return;
    textbuf[text_len++] = c;
    textbuf[text_len] = '\0';

    if (c == '\n') {
        cursor_x = 0;
        cursor_y += line_height;
    } else {
        cursor_x += 8 * 2; // size=2
        if (cursor_x > screen_width - 20) {
            cursor_x = 0;
            cursor_y += line_height;
        }
    }
}

// ============================================================================
// BACKSPACE
// ============================================================================
static void editor_backspace() {
    if (text_len <= 0) return;

    char last = textbuf[text_len - 1];
    text_len--;
    textbuf[text_len] = '\0';

    if (last == '\n') {
        // recompute cursor from scratch
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

    // normal char
    cursor_x -= 8 * 2;
    if (cursor_x < 0) cursor_x = 0;
}


// ============================================================================
// SAVE FILE (Ctrl+S)
// ============================================================================
static void editor_save() {
    // General filename for now
    const char* fname = "NOTE.TXT";

    int ok = fat_create_file_root(fname,
                                  (const uint8_t*)textbuf,
                                  (uint32_t)text_len);

    // give small feedback
    if (ok == 0) {
        draw_text(20, screen_height- 30, "Saved NOTE.TXT", 0xFFFFFF, 2);
    } else {
        draw_text(20, screen_height - 30, "Save FAILED", 0xFF0000, 2);
    }
    flip();
}


// ============================================================================
// MAIN TEXT EDITOR LOOP
// ============================================================================
void text_editor_main() {

    // reset buffer
    text_len = 0;
    cursor_x = 0;
    cursor_y = 0;
    textbuf[0] = '\0';

    uint32_t last_frame = 0;

    while (1) {

        // -----------------------
        // INPUT HANDLING
        // -----------------------

        // Ctrl+S → SAVE
        if (key_state[0x1F] && key_state[0x1F] && key_state[0x1F]) { /* dummy lol */ }
        // proper check:
        // Ctrl = scancode 0x1D, S = 0x1F
        if (key_state[0x1D] && key_state[0x1F]) {
            editor_save();
        }

        for (char c = 'a'; c <= 'z'; c++) {
            if (is_pressed(c)) {
                editor_insert_char(c);
                while (is_pressed(c)); // wait until released
            }
        }

        if (is_pressed(' ')) {
            editor_insert_char(' ');
            while (is_pressed(' '));
        }

        if (is_pressed('\n')) {
            editor_insert_char('\n');
            while (is_pressed('\n'));
        }

        if (is_pressed('\b')) {  // backspace
            editor_backspace();
            while (is_pressed('\b'));
        }

        // -----------------------
        // DRAW
        // -----------------------
        if (tick - last_frame >= 2) {
            last_frame = tick;

            screen_clear();
            draw_text(10, 10, textbuf, 0xFFFFFF, 2);

            // draw cursor (simple rectangle)
            draw_text(cursor_x + 10, cursor_y + 10, "_", 0x00FF00, 2);

            flip();
        }
    }
}
