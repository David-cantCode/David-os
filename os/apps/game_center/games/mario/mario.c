
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





//globals
struct Player player;
static int is_in_menu;
int menu_options = 2;
int pointer = 1; 
static int end_game;
static int current_level;




//settings
static int fps = 30;
static int screen_w = 600;
static int screen_h = 600;
static int gravity = 2;
static int mario_offset = 50; //height offset so hes no clipping into tiles

//colors
static uint32_t red = 0x00000FFFF;
static uint32_t dark_brown = 0x8000FF8F;
static uint32_t light_brown = 0x3FFFFF3F;




//sprites:
Sprite mario_sprite;


//levels
#define TILE_SIZE 40
    
//rules:
    //0 air, 1 brick, 2 question block

    //level 1
    #define l1_rows 15
    #define l1_cols 20

    int l1_map[l1_rows][l1_cols] = {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0}, 
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
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


void render_level(int* level, int level_cols, int level_rows) {
    for (int row = 0; row < level_rows; row++) {
        for (int col = 0; col < level_cols; col++) {
            
          
            int current_index = (row * level_cols) + col;

        
            int tile_id = level[current_index];

            
            if (tile_id > 0) {
                int x_pos = col * TILE_SIZE;
                int y_pos = row * TILE_SIZE;
                
                uint32_t color = (tile_id == 1) ? dark_brown : light_brown;
               
                draw_rect(x_pos, y_pos, TILE_SIZE, TILE_SIZE, color); 
            }
        }
    }
}

void update_physics(int* level, int level_cols, int level_rows) {
   
    player.velocity.y += gravity; 

    //apply velocity to pos
    player.position.y += player.velocity.y;
    player.position.x += player.velocity.x; 

    //floor check
    int map_col = (player.position.x + 15) / TILE_SIZE; //mario center
    int map_row = (player.position.y + mario_offset) / TILE_SIZE;

    //check bounds
    if (map_row >= 0 && map_row < level_rows && map_col >= 0 && map_col < level_cols) {

        int tile_index = (map_row * level_cols) + map_col;
        
        //if air
        if (level[tile_index] != 0) {
            
            //snap mario
            player.position.y = (map_row * TILE_SIZE) - mario_offset; 
            player.velocity.y = 0;
            player.is_on_ground = 1;
        } else {
            player.is_on_ground = 0;
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
        player.velocity.y = -20; 
        player.is_on_ground = 0;
    }
}

static void main_loop() { 
    while (end_game == 0) {
        if (should_update()) {
            screen_clear();
            input_poll();
            if (is_in_menu==1) menu();

            else{

            input_poll();
            update_physics(l1_map, l1_cols, l1_rows); 
            render_level(l1_map, l1_cols, l1_rows);  
            draw_sprite(player.position.x, player.position.y, mario_sprite, 3);
            
        
            }

            
            flip(); 
        }
    }
}

void mario_on_start(){
    set_fps(fps);
    is_in_menu = 1;
    
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


    main_loop();




}


