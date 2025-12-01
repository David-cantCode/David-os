
#include "../../../../libary/include/davidgl.h"
#include "../../../../libary/include/util.h"

#include <stdint.h>
#include "mario.h"


//structs 
struct Vector2{
    int x, y;
};


struct Camera{
    struct Vector2 position;
};

struct Player{
    struct Vector2 position;
    struct Vector2 velocity;
    int is_on_ground;

};


struct Enemy{
    struct Vector2 position;
    struct Vector2 velocity;
    int is_alive;
    int is_on_ground;
};

#define MAX_ENEMIES 100
struct Enemy enemies[MAX_ENEMIES];
static int num_enemies = 0;




//globals
struct Camera camera;
struct Player player;

static int is_in_menu;
static int menu_options = 2;
static int pointer = 1; 
static int end_game;
static int current_level;
static char* buf;



//settings
static int fps = 30;
static int screen_w = 600;
static int screen_h = 600;
static int gravity = 2;
static int mario_offset = 35; //height offset so hes no clipping into tiles
static int jump_str = 25;


static int lives = 3; 
static int score; 
static int coins;




//colors
static uint32_t red = 0xF8000000;  
static uint32_t dark_brown = 0x8000FF8F;
static uint32_t light_brown = 0xFF000FF0;
static uint32_t black = 0x00000000;
static uint32_t white = 0xFFFFFFFF;


//sprites:
static Sprite mario_sprite;
static Sprite ground_sprite;
static Sprite p_block_sprite;
static Sprite goomba_sprite;

//screen pos = world pos - camera pos

//levels
#define TILE_SIZE 34




    
//rules:
    //0 air, 1 brick, 2 question block

    //level 1
    #define l1_rows 18
    #define l1_cols 61

    int l1_map[l1_rows][l1_cols] = {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };





static int menu(){
    //ret -1 if not chosen yet| ret 0 -> menu_options on select


    // 3 options ; play, exit



    if (is_pressed('s')){
        pointer ++;

    }
   
    else if (is_pressed('w')){
        pointer --;

    } 
    //clamp pointer
    if (pointer<1){pointer = 1;} 
    if (pointer>menu_options){pointer = menu_options;} 


    if (is_pressed(' ')&& pointer ==2){
        end_game = 1;
    }

    if (is_pressed(' ')&& pointer == 1){
        is_in_menu =0;
        return 0;
    }


    //draw menu, const c


    draw_text(screen_w / 4, 50, "SUPER MARIO", 0xFF000FF0, 3);
    char buff[4];
    s_printf(buff, "pointer = %d", pointer);
    draw_text(screen_w / 2, 300, buff, 0xFF000FF0, 3);


    if (pointer ==1){
                draw_text(screen_w / 4, 300, "Play", 0xFFFFFFFF, 2);
                draw_text(screen_w / 4, 350, "Exit", 0xFF000FF0, 2);

    }

    else{
                draw_text(screen_w / 4, 300, "Play", 0xFF000FF0, 2);
                draw_text(screen_w / 4, 350, "Exit", 0xFFFFFFFF, 2);


    }


    return -1;
}


static void draw_ui(){

    s_printf(buf, "Score: %d", score);
    draw_text(20, 10, buf, 0xFFFFFFFF, 2);
    
    //drawsprite(coins)
    //s_printf(buf, "x%d", coins);

}


//***************************
//**********ENEMIES************
//***************************
static void add_enemy(int x, int y) {
    if (num_enemies < MAX_ENEMIES) {
        enemies[num_enemies].position.x = x;
        enemies[num_enemies].position.y = y;
        enemies[num_enemies].velocity.x = -2; 
        enemies[num_enemies].velocity.y = 0;
        enemies[num_enemies].is_alive = 1;
        enemies[num_enemies].is_on_ground = 0;
        num_enemies++;
    }
}


static void render_enemies() {
    for (int i = 0; i < num_enemies; i++) {
        if (enemies[i].is_alive ==0) continue;

        int screen_x = enemies[i].position.x - camera.position.x;
        int screen_y = enemies[i].position.y - camera.position.y;


        draw_sprite(screen_x, screen_y, goomba_sprite, 2); 
      
    }
}


static void enemy_physics(){


    
}



//***************************
//**********LEVEL************
//***************************
static void innit_level(){
    num_enemies = 0;

    add_enemy(100, 200);

}

static void render_level(int* level, int level_cols, int level_rows) {
    for (int row = 0; row < level_rows; row++) {
        for (int col = 0; col < level_cols; col++) {
            
            int current_index = (row * level_cols) + col;
            int tile_id = level[current_index];

            if (tile_id > 0) {
                //get wlrd pos
                int world_x = col * TILE_SIZE;
                int world_y = row * TILE_SIZE;

                //convert to screen pos
                int screen_x = world_x - camera.position.x;
                int screen_y = world_y - camera.position.y;
                
                if (screen_x + TILE_SIZE > 0 && screen_x < screen_w) {
                   
                if (tile_id ==1) draw_sprite(screen_x, screen_y, ground_sprite, 4);
                if (tile_id ==2) draw_sprite(screen_x, screen_y, p_block_sprite, 4);
                }
            }
        }
    }
}


void update_camera(int level_cols) {
    //add camera y pos update in future 


    camera.position.x = player.position.x - (screen_w / 2);    

    if (camera.position.x < 0) camera.position.x = 0;


    int level_width_pixels = level_cols * TILE_SIZE;
    if (camera.position.x > level_width_pixels - screen_w) {
        camera.position.x = level_width_pixels - screen_w;
    }
}



int get_tile(int x, int y, int* level, int cols, int rows) {
    //rets tile if x, y == tile.pos


    int col = x / TILE_SIZE;
    int row = y / TILE_SIZE;

    if (col < 0 || col >= cols || row < 0 || row >= rows) {
        return 0; 
    }

    return level[row * cols + col];
}

void update_physics(int* level, int level_cols, int level_rows) {

    //why dont i just use one variable since there the same? idk haha
    int player_w = 32; 
    int player_h = 32; 

    //x axis movement and collison
    player.position.x += player.velocity.x;

    //check right side
    if (player.velocity.x > 0) {
    
        int tile_top = get_tile(player.position.x + player_w, player.position.y, level, level_cols, level_rows);
        int tile_bot = get_tile(player.position.x + player_w, player.position.y + player_h - 1, level, level_cols, level_rows);

        //on collide
        if (tile_top != 0 || tile_bot != 0) {
            int tile_x = ((player.position.x + player_w) / TILE_SIZE) * TILE_SIZE;
            player.position.x = tile_x - player_w - 1; 
            player.velocity.x = 0;
        }
    }


    //check left
    else if (player.velocity.x < 0) {
      
        int tile_top = get_tile(player.position.x, player.position.y, level, level_cols, level_rows);
        int tile_bot = get_tile(player.position.x, player.position.y + player_h - 1, level, level_cols, level_rows);

        //on hit
        if (tile_top != 0 || tile_bot != 0) {
            int tile_x = (player.position.x / TILE_SIZE) * TILE_SIZE;
            player.position.x = tile_x + TILE_SIZE + 1;
            player.velocity.x = 0;
        }
    }

    //y axis collision n movement
    player.velocity.y += gravity; 
    player.position.y += player.velocity.y;
    player.is_on_ground = 0; 


    //floor_check
    if (player.velocity.y > 0) {
        int tile_left = get_tile(player.position.x + 4, player.position.y + player_h, level, level_cols, level_rows);
        int tile_right = get_tile(player.position.x + player_w - 4, player.position.y + player_h, level, level_cols, level_rows);
        //+- 4; lil offset

        if (tile_left != 0 || tile_right != 0) {
            //on ground
            int tile_y = ((player.position.y + player_h) / TILE_SIZE) * TILE_SIZE;
            player.position.y = tile_y - player_h;
            player.velocity.y = 0;
            player.is_on_ground = 1;
        }
    }


    //ceiling check
    else if (player.velocity.y < 0) {
       
        int tile_left = get_tile(player.position.x + 4, player.position.y, level, level_cols, level_rows);
        int tile_right = get_tile(player.position.x + player_w - 4, player.position.y, level, level_cols, level_rows);

        if (tile_left != 0 || tile_right != 0) {
            //hit head
            int tile_y = (player.position.y / TILE_SIZE) * TILE_SIZE;
            player.position.y = tile_y + TILE_SIZE + 1;
            player.velocity.y = 0;
        }
    }
}



static void input_poll(){
    if (is_in_menu==1){return;}
    
    player.velocity.x = 0; 
    int speed = 8;

    if (is_pressed('d')){
        player.velocity.x = speed;
    }

    if (is_pressed('a')){
        player.velocity.x = -speed;
    }


    if (is_pressed(' ') && player.is_on_ground){
        player.velocity.y = -jump_str; 
        player.is_on_ground = 0;
    }
}




static void main_loop() { 
    while (end_game == 0) {
        if (should_update()) {
            screen_clear();
            input_poll();
            if (is_in_menu==1) {
                if (menu() == 0) innit_level();}

            else{

            input_poll();
            update_physics(l1_map, l1_cols, l1_rows); 
            update_camera(l1_cols);
            render_level(l1_map, l1_cols, l1_rows);  
            render_enemies();
            
            
            draw_ui();

            draw_sprite(player.position.x- camera.position.x, player.position.y - camera.position.y, mario_sprite, 2);
            
        
            }

            
            flip(); 
        }
    }
}

void mario_on_start(){
    set_fps(fps);
    is_in_menu = 1;
    end_game= 0;
    
    //i wrote this all by hand coping a mario sprite
    uint32_t mario_pixles[256] = {
    0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF, red, red, red, red, red, 0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF,
    0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF,  red, red, red, red, red, red, red, red, 0x000000FF, 0x000000FF, 0x000000FF,
    0x000000FF, 0x000000FF,  0x000000FF, 0x000000FF, dark_brown, dark_brown, dark_brown, light_brown, light_brown, dark_brown, light_brown, 0x000000FF, 0x000000FF, 0x000000FF,0x000000FF, 0x000000FF,
    0x000000FF, 0x000000FF, 0x000000FF, dark_brown, light_brown, dark_brown, light_brown, light_brown, light_brown, dark_brown, light_brown, light_brown, light_brown,0x000000FF, 0x000000FF,0x000000FF, 
    0x000000FF, 0x000000FF, 0x000000FF,dark_brown, light_brown, dark_brown, dark_brown, light_brown, light_brown, light_brown, dark_brown, light_brown, light_brown, light_brown,  0x000000FF,0x000000FF,
    0x000000FF, 0x000000FF, 0x000000FF, dark_brown, dark_brown, light_brown, light_brown, light_brown, light_brown, dark_brown, dark_brown,  dark_brown, dark_brown, 0x000000FF,0x000000FF, 0x000000FF,
    0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF, light_brown, light_brown, light_brown, light_brown, light_brown, light_brown, light_brown, 0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF, 
 
    //body start

    0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF, dark_brown, dark_brown, red, dark_brown, dark_brown, dark_brown,0x000000FF, 0x000000FF, 0x000000FF,0x000000FF, 0x000000FF,0x000000FF,
    0x000000FF, 0x000000FF, 0x000000FF, dark_brown, dark_brown, dark_brown, red, dark_brown, dark_brown, red, dark_brown,dark_brown,dark_brown, 0x000000FF, 0x000000FF, 0x000000FF,
    0x000000FF, 0x000000FF, dark_brown, dark_brown, dark_brown, dark_brown, red, red, red, red, dark_brown, dark_brown ,dark_brown, dark_brown, 0x000000FF, 0x000000FF,
    
    //hands
    0x000000FF, 0x000000FF, light_brown, light_brown, dark_brown, red, light_brown, red, red, light_brown, red, dark_brown, light_brown, light_brown, 0x000000FF,  0x000000FF, 
    0x000000FF, 0x000000FF, light_brown, light_brown, light_brown, red, red, red, red, red, red, light_brown, light_brown, light_brown, 0x000000FF, 0x000000FF,
    0x000000FF, 0x000000FF, light_brown, light_brown, red, red, red, red, red, red, red, red,light_brown, light_brown, 0x000000FF, 0x000000FF,
   
   
   
    0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF, red, red, red, 0x000000FF, 0x000000FF, red, red, red, 0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF,
    
    //feet
    0x000000FF, 0x000000FF, 0x000000FF, dark_brown, dark_brown, dark_brown, 0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF,dark_brown,dark_brown, dark_brown, 0x000000FF, 0x000000FF,0x000000FF, 
    0x000000FF, 0x000000FF, dark_brown, dark_brown, dark_brown, dark_brown, 0x000000FF, 0x000000FF,0x000000FF, 0x000000FF,dark_brown, dark_brown, dark_brown, dark_brown,  0x000000FF, 0x000000FF
    };

    mario_sprite.pixles = mario_pixles;
    mario_sprite. width = 16; mario_sprite.height = 16;


    uint32_t ground_pixles[64]= {
        dark_brown, dark_brown, dark_brown,  dark_brown,  dark_brown,  black, dark_brown,  dark_brown,
        dark_brown, dark_brown, dark_brown,  dark_brown,  dark_brown,  black, dark_brown,  dark_brown,
        dark_brown, dark_brown, dark_brown,  dark_brown,  dark_brown,  black, black, black,
        dark_brown, dark_brown, dark_brown,  dark_brown,  dark_brown,  black, dark_brown,  dark_brown,
        dark_brown, dark_brown, dark_brown,  dark_brown,  dark_brown,  black, dark_brown,  dark_brown,
        black, dark_brown, dark_brown,  dark_brown,  black,  dark_brown, dark_brown,  dark_brown,
        dark_brown, black, black,  black,  black,  dark_brown, dark_brown,  dark_brown,
        dark_brown, dark_brown, black,  dark_brown,  dark_brown,  dark_brown, dark_brown,  dark_brown,
    };
    ground_sprite.pixles = ground_pixles;
    ground_sprite.width = 8;
    ground_sprite.height = 8;


    uint32_t p_block_pixles[64]= {

         dark_brown, light_brown, white, white, white, white, light_brown, dark_brown, 
       light_brown, white, white, white, white, white, white, light_brown, 
         light_brown, white, white, light_brown, light_brown, white, white, light_brown, 
         light_brown, light_brown, light_brown, light_brown, light_brown, white, white, light_brown, 
         light_brown, light_brown, light_brown, light_brown, white, white, white, light_brown,
       light_brown, light_brown, light_brown, white, white, light_brown, light_brown, light_brown, 
         light_brown, light_brown, light_brown, light_brown, light_brown, light_brown, light_brown, light_brown, 
      dark_brown, light_brown, light_brown, white, white, light_brown, light_brown, dark_brown
    };

    p_block_sprite.pixles = p_block_pixles;
    p_block_sprite.width = 8;
    p_block_sprite.height = 8;


    uint32_t goomba_pixles[100]={
        0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF, dark_brown, dark_brown, 0x000000FF, 0x000000FF,0x000000FF,0x000000FF,
        0x000000FF, 0x000000FF, dark_brown, dark_brown, dark_brown, dark_brown, dark_brown, dark_brown, 0x000000FF,0x000000FF,
        0x000000FF,black, black, black, dark_brown, dark_brown, black, black, black, 0x000000FF,
        dark_brown, dark_brown, light_brown, black, black, black, black, light_brown, dark_brown, dark_brown,
        dark_brown, dark_brown, light_brown, light_brown, black, black, light_brown, light_brown, dark_brown,dark_brown,
        dark_brown, dark_brown,dark_brown,dark_brown,dark_brown,dark_brown,dark_brown,dark_brown,dark_brown,dark_brown,
        0x000000FF, dark_brown, dark_brown, light_brown, light_brown, light_brown, light_brown, dark_brown, dark_brown, 0x000000FF,
        black, black, light_brown, light_brown, light_brown, light_brown, light_brown, light_brown, black,black,
        black, black, black, black, light_brown, light_brown, black, black, black, black,
        0x000000FF, black, black, black, 0x000000FF, 0x000000FF, black, black, black, 0x000000FF
    };
    goomba_sprite.pixles = goomba_pixles;
    goomba_sprite.height = 10;
    goomba_sprite.width = 10;


    main_loop();

}


