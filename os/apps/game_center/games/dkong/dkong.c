#include "../../../../libary/include/davidgl.h"
#include "../../../../libary/include/util.h"

#include <stdint.h>


struct Vector2{
    int x, y;
};


struct Player{
    struct Vector2 position;
    struct Vector2 velocity;
    int is_on_ground;
    int is_climbing;

};


struct Barrel{
    struct Vector2 position;
    struct Vector2 velocity;

    int type;
};

//globals
#define MAX_BARRELS 100
struct Barrel barrels[MAX_BARRELS];
static int end_game;
static int is_in_menu;



//settings
int fps = 30;
static int screen_w = 600;
static int screen_h = 600;

//colors
static uint32_t red = 0xF8000000;
static uint32_t light = 0xFF000FF0;
static uint32_t blue = 0x0000880;
static uint32_t white = 0xFFFFFFFF;
static uint32_t empty = 0x000000FF;
static uint32_t pink = 0x55000000;


//sprites
static Sprite dkong_sprite;
static Sprite platform_sprite;
static Sprite ladder_sprite;

//map
#define TILE_SIZE 20

#define COLS 10
#define ROWS 35

//0 nothing
//1 ladder
// 2 platform  3 climable platform , 
// 4 platform slopped up to right, 5 climable platform slopped up to the right,
// 6 platform slopped up to the left, 7 climable platform slopped up to the left

//confusing, yes
//does the job, also yes
//is there a better way, prolly?
//am i ever gonna change it, nah

int map[ROWS][COLS]= {
    //platforms are seperated with spaces for readabbility (even thos its still not readable lol unless your crazy like me)
    
    {0, 0 ,0 ,0 ,1 ,0 ,0 ,0 ,0, 0},
    {0, 0 ,0 ,0 ,1 ,0 ,0 ,0 ,0, 0},
    {0, 0 ,0 ,0 ,1 ,0 ,0 ,0 ,0, 0},
    {0, 0 ,0 ,0 ,1 ,1 ,3 ,0 ,0, 0},
    {0, 0 ,0 ,0 ,1 ,0 ,2 ,0 ,0, 0},
    {0, 0 ,0 ,0 ,1 ,0 ,2 ,0 ,0, 0},

    //where dkong is 
    {2, 2 ,2 ,2 ,3 ,6 ,6 ,6 ,7, 0},
    
    {0, 0 ,0 ,0 ,0 ,0 ,0 ,0 ,1, 0},
    {0, 0 ,0 ,0 ,1 ,0 ,0 ,0 ,1, 0},

    {0, 5 ,4 ,5 ,4, 4, 4 ,5 ,4, 4},
    
    {0, 1 ,0 ,1 ,0 ,0 ,0 ,0 ,0, 0},
    {0, 1 ,0 ,1 ,0 ,0 ,0 ,0 ,0, 0},

    {6, 6, 7, 6, 6, 7, 6, 6, 6, 0},
    
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 1, 0, 0, 0, 0},

    {0, 5, 4, 4, 4, 4, 4, 4, 4, 4},

    {0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 1, 0, 0, 0},

    {6, 6, 6, 7, 6, 6, 6, 6, 7, 0},

    {0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {0, 0, 0, 1, 0, 0, 0, 0, 1, 0},

    {0, 5, 4, 4, 4, 4, 4, 5, 4, 4},
    
    {0, 1, 0, 0, 0, 0, 0, 1, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 1, 0, 0},
    
    {6, 6, 6, 7, 6, 7, 6, 6, 7, 0},

    {0, 0, 0, 0, 0, 1, 0, 0, 1, 0},
    {0, 0, 0, 1, 0, 1, 0, 0, 1, 0},
    
    {0, 5, 4, 4, 4, 5, 4, 4, 4, 4},

    {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, 
    {0, 1, 0, 0, 0, 1, 0, 0, 0, 0},

    {6, 6, 6, 7, 6, 6, 6, 6, 7, 0},

    {0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {0, 0, 0, 1, 0, 0, 0, 0, 1, 0},

    //start
    {1, 1, 1, 1, 4, 4, 4, 4, 4, 4}

};

static void render_map(){
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            
            int tile_id = map[row][col];

            if (tile_id > 0) {
                int pos_x = col * TILE_SIZE+ 200;
                int pos_y = row * TILE_SIZE;
                  
                if (tile_id == 1)draw_sprite(pos_x, pos_y, ladder_sprite, 2);

                else{
                draw_sprite(pos_x, pos_y, platform_sprite, 2);}
                }
            }
        }
    }































static void main_loop() { 

    while (1) {
        if (should_update()) {
            screen_clear();
           
            render_map();
            draw_sprite(screen_w /2, screen_h/2, dkong_sprite, 3);
           // draw_sprite(50, 50, platform_sprite, 3);            
            
            flip(); 
        }
    }
}

static void innit_sprites(){
    uint32_t dkong_pixles[1280]={
        empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, red, red, red, red, red, red, red,empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty,
        empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, red, red, red, red, red, red, red, red, red, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty,
        empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, red, red, light, light, red, red, red, light, light, red, red, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty,
        empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, red, red, light, light, light, light, red, light, light, light, light, red, red, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty,
    
        //eyes 
        empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, light, light, red, red, light, light, light, white, white, light, white, white, light, light, light, light, red, light, light, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty,
        empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, light, light, light, red, red, red, light, light, white, blue, light, blue, white, light, light, red, red, light, light, light, empty, empty, empty, empty, empty, empty, empty, empty, empty, empty,
        
        empty, empty, empty, empty, empty, empty, light, red, red, red, light, light, light, red, red, red, light, light, light, light, light, light, light, light, light, red, red, light, light, light, red, red, red, light, empty, empty, empty, empty, empty, empty,
        
        //nose
        empty, empty, empty, empty, empty,  light, red,red, red, light, red, red, light, light, light, light, light, light, light, red, red, red, light, light, light, light, light, light, red, red, light, red, red, red, light, empty, empty, empty, empty, empty,
        
        
    
    };

    dkong_sprite.pixles = dkong_pixles;
    dkong_sprite.height = 8;
    dkong_sprite.width = 40;

    

    uint32_t platform_pxiles[128]={
        pink, pink, pink, pink, pink, pink, pink, pink, pink, pink, pink, pink, pink, pink, pink, pink, 
        red, red, red, red,red, red, red, red,red, red, red, red, red, red, red, red,
        empty, empty, pink, pink, pink, empty, empty, empty, empty, empty, pink, pink, pink, empty, empty, empty,
        empty, pink, pink, empty, pink, pink, empty, empty, empty, pink, pink, empty, pink, pink, empty, empty,
        pink, pink, empty, empty, empty, pink, pink, empty, pink, pink, empty, empty, empty, pink, pink, empty,
        pink, empty, empty, empty, empty, empty, pink, pink, pink, empty, empty, empty, empty, empty, pink, pink,
        pink, pink, pink, pink, pink, pink, pink, pink, pink, pink, pink, pink, pink, pink, pink, pink, 
        red, red, red, red,red, red, red, red,red, red, red, red, red, red, red, red,
    };

    platform_sprite.pixles = platform_pxiles;
    platform_sprite.width = 16;
    platform_sprite.height = 8;


    uint32_t  ladder_pixles[64]={
        white, empty, empty, empty, empty, empty, empty, white,
        white, empty, empty, empty, empty, empty, empty, white,
        white, white, white, white, white, white, white, white,
        white, empty, empty, empty, empty, empty, empty, white,
        white, empty, empty, empty, empty, empty, empty, white,
        white, empty, empty, empty, empty, empty, empty, white,
        white, white, white, white, white, white, white, white,
        white, empty, empty, empty, empty, empty, empty, white,
    };
    ladder_sprite.pixles = ladder_pixles;
    ladder_sprite.height = 8;
    ladder_sprite.width = 8;


}



void dkong_on_start(){
    set_fps(30);
    is_in_menu = 1;
    end_game= 0;

    innit_sprites();
    main_loop();

}