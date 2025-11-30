
#include "../../../../libary/include/davidgl.h"
#include "../../../../libary/include/util.h"
#include <stdint.h>
#include "mario.h"

struct Vector2{
    int x, y;
};


struct Camera{
    struct Vector2 position;
};

struct Player{
    struct Vector2 position;
    int is_on_ground;

};


static int fps = 30;
static int is_in_menu;

static int screen_w = 600;
static int screen_h = 600;

int menu_options = 2;
int pointer = 1; 
static int end_game;

static int current_world;

//colors

static uint32_t red = 0x00000FFFF;
static uint32_t dark_brown = 0x8000FF8F;
static uint32_t light_brown = 0x3FFFFF3F;




//sprites:
Sprite mario_sprite;








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

    if (is_pressed(' ')&& pointer ==3){
        end_game = 1;
    }

    if (is_pressed(' ')){
        return pointer;
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



static void main_loop() { 
    while (end_game == 0) {
        if (should_update()) {
            screen_clear();
        
            if (is_in_menu==1) menu();

            draw_sprite(25, 25, mario_sprite, 10);


            
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


