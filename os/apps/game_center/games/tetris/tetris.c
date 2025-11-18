
#include <stdint.h>
#include "util.h" 

extern volatile unsigned int tick;
extern uint8_t key_state[256];
extern uint32_t screen_width;
extern uint32_t screen_height;

void set_fps(unsigned int fps);
int should_update();
void screen_clear();
void flip();
int is_pressed(char c);
void draw_rect(int x, int y, int w, int h, uint32_t color);
void draw_text(int x, int y, const char* str, uint32_t color, int size);


#define PLAY_W 10
#define PLAY_H 20
#define BLOCK 24            
#define PLAY_X 200          
#define PLAY_Y 40


static uint8_t board[PLAY_H][PLAY_W];


static const char tetromino[7][4][16] = {
// I
{
"0000111100000000",
"0010001000100010",
"0000111100000000",
"0100010001000100",
},
// J
{
"1000111000000000",
"0100010001100000",
"0000111001000000",
"0110001000100000",
},
// L
{
"0010111000000000",
"0100010001001000",
"0000111010000000",
"0010010001000000",
},
// O
{
"0110011000000000",
"0110011000000000",
"0110011000000000",
"0110011000000000",
},
// S
{
"0110110000000000",
"0100011001000000",
"0000011011000000",
"0010011000100000",
},
// T
{
"0100111000000000",
"0100011000100000",
"0000111010000000",
"0100110000100000",
},
// Z
{
"1100011000000000",
"0100010001001000",
"0000110001100000",
"0010010001000000",
}
};


static int cur_type = 0;
static int cur_rot = 0;
static int cur_x = 3; 
static int cur_y = 0; 

static int game_over = 0;
static int score = 0;
static int lines_cleared = 0;
static int level = 1;


static int gravity_counter = 0;
static int gravity_speed = 30; //ticks per drop will reduce with level


static int tetromino_cell(int type, int rot, int x, int y) {

    return tetromino[type][rot][y*4 + x] == '1';
}

static int check_collision(int nx, int ny, int nrot) {
    for (int py = 0; py < 4; py++) {
        for (int px = 0; px < 4; px++) {
            if (!tetromino_cell(cur_type, nrot, px, py)) continue;
            int bx = nx + px;
            int by = ny + py;
            if (bx < 0 || bx >= PLAY_W || by >= PLAY_H) return 1; //out of bounds
            if (by >= 0 && board[by][bx]) return 1; //collides with filled cell
        }
    }
    return 0;
}

static void lock_piece() {
    for (int py = 0; py < 4; py++) {
        for (int px = 0; px < 4; px++) {
            if (!tetromino_cell(cur_type, cur_rot, px, py)) continue;
            int bx = cur_x + px;
            int by = cur_y + py;
            if (by >= 0 && by < PLAY_H && bx >= 0 && bx < PLAY_W) {
                board[by][bx] = cur_type + 1; //store 1..7
            }
        }
    }
}

static void clear_lines() {
    int cleared = 0;
    for (int y = PLAY_H - 1; y >= 0; y--) {
        int full = 1;
        for (int x = 0; x < PLAY_W; x++) if (!board[y][x]) { full = 0; break; }
        if (full) {
            //move everything above down
            for (int yy = y; yy > 0; yy--) memorycpy(board[yy], board[yy-1], PLAY_W);
            memoryset(board[0], 0, PLAY_W);
            cleared++;
            y++; //recheck same row (moved down)
        }
    }
    if (cleared) {
        lines_cleared += cleared;
        //scoring loosely follows classic: 40,100,300,1200 times level
        static const int base[] = {0, 40, 100, 300, 1200};
        score += base[cleared] * level;
        //increase level every 10 lines
        level = 1 + (lines_cleared / 10);
        //speed up
        gravity_speed = 30 - (level - 1) * 2;
        if (gravity_speed < 4) gravity_speed = 4;
    }
}

static void spawn_piece() {
    cur_type = (tick % 7); 
    cur_rot = 0;
    cur_x = 3;
    cur_y = -2; 
    if (check_collision(cur_x, cur_y, cur_rot)) {
        game_over = 1;
    }
}


static void draw_block_at(int bx, int by, int type) {
    int px = PLAY_X + bx * BLOCK;
    int py = PLAY_Y + by * BLOCK;
    
    switch (type) { //the colors are all wrong lol but idc
        case 1: draw_rect(px+1, py+1, BLOCK-2, BLOCK-2, 0x00FFFF00); break; // yellow O
        case 2: draw_rect(px+1, py+1, BLOCK-2, BLOCK-2, 0x00FF0000); break; // red I
        case 3: draw_rect(px+1, py+1, BLOCK-2, BLOCK-2, 0x0000FF00); break; // green S
        case 4: draw_rect(px+1, py+1, BLOCK-2, BLOCK-2, 0x0000FFFF); break; // cyan J
        case 5: draw_rect(px+1, py+1, BLOCK-2, BLOCK-2, 0x00FF00FF); break; // magenta L
        case 6: draw_rect(px+1, py+1, BLOCK-2, BLOCK-2, 0x000000FF); break; // blue T
        case 7: draw_rect(px+1, py+1, BLOCK-2, BLOCK-2, 0x00FFFFFF); break; // white Z
        default: break;
    }
}

static void draw_playfield() {
    //background
    draw_rect(PLAY_X - 4, PLAY_Y - 4, PLAY_W * BLOCK + 8, PLAY_H * BLOCK + 8, 0x00202020);

    for (int y = 0; y < PLAY_H; y++) {
        for (int x = 0; x < PLAY_W; x++) {
            if (board[y][x]) draw_block_at(x, y, board[y][x]);
            else {
              
                draw_rect(PLAY_X + x*BLOCK + 1, PLAY_Y + y*BLOCK + 1, BLOCK-2, BLOCK-2, 0x00101010);
            }
        }
    }
}

static void draw_current_piece() {
    for (int py = 0; py < 4; py++) for (int px = 0; px < 4; px++) {
        if (!tetromino_cell(cur_type, cur_rot, px, py)) continue;
        int bx = cur_x + px;
        int by = cur_y + py;
        if (by >= 0 && bx >= 0 && bx < PLAY_W && by < PLAY_H) draw_block_at(bx, by, cur_type + 1);
    }
}

static void draw_ui() {
}

static void tick_game() {
    if (game_over) return;

  
    static int left_held = 0, right_held = 0;
    if (is_pressed('a')) {
        if (!left_held) {
            if (!check_collision(cur_x - 1, cur_y, cur_rot)) cur_x--;
        }
        left_held = 1;
    } else left_held = 0;

    if (is_pressed('d')) {
        if (!right_held) {
            if (!check_collision(cur_x + 1, cur_y, cur_rot)) cur_x++;
        }
        right_held = 1;
    } else right_held = 0;

  
  
  
    //rotate
    static int rot_held = 0;
    if (is_pressed('w')) {
        if (!rot_held) {
            int nr = (cur_rot + 1) & 3;
            if (!check_collision(cur_x, cur_y, nr)) cur_rot = nr;
            else {
                
                if (!check_collision(cur_x - 1, cur_y, nr)) cur_x--, cur_rot = nr;
                else if (!check_collision(cur_x + 1, cur_y, nr)) cur_x++, cur_rot = nr;
            }
        }
        rot_held = 1;
    } else rot_held = 0;

    //soft drop
    if (is_pressed('s')) {
        if (!check_collision(cur_x, cur_y + 1, cur_rot)) cur_y++;
        else {
            lock_piece();
            clear_lines();
            spawn_piece();
        }
    }

    //hard drop
    static int space_held = 0;
    if (is_pressed(' ')) {
        if (!space_held) {
            while (!check_collision(cur_x, cur_y + 1, cur_rot)) cur_y++;
            lock_piece();
            clear_lines();
            spawn_piece();
        }
        space_held = 1;
    } else space_held = 0;

    //quit
    if (is_pressed('q')) game_over = 1;

    //gravity
    gravity_counter++;
    if (gravity_counter >= gravity_speed) {
        gravity_counter = 0;
        if (!check_collision(cur_x, cur_y + 1, cur_rot)) {
            cur_y++;
        } else {
          
            lock_piece();
            clear_lines();
            spawn_piece();
        }
    }
}


static void main_loop() {
    set_fps(60);
    spawn_piece();

    while (1) {
        if (should_update()) {
      
            tick_game();

       
            screen_clear();
            draw_playfield();
            draw_current_piece();
            draw_ui();


            flip();


            if (game_over && is_pressed('q')) return;
        }
    }
}


void tetris_main() {
    memoryset(board, 0, sizeof(board));
    game_over = 0;
    score = 0;
    lines_cleared = 0;
    level = 1;
    gravity_speed = 30;
    main_loop();
    
}
