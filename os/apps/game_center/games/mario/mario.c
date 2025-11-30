
#include "../../../../libary/include/davidgl.h"
#include "../../../../libary/include/util.h"
int fps = 30;
int is_in_menu;

static int screen_w = 600;
static int screen_h = 600;

int menu_options = 2;
int pointer = 1; 
static int end_game;

int current_world;

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



            
            flip(); 
        }
    }
}

void mario_on_start(){
    set_fps(fps);
    is_in_menu = 1;
    
    main_loop();




}


