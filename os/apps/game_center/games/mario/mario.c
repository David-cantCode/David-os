
#include "../../../../libary/include/davidgl.h"
int fps = 30;
int is_in_menu;

static int screen_w = 600;
static int screen_h = 600;

int menu_options = 3;
static int menu(){
    //ret -1 if not chosen yet| ret 0 -> menu_options on select


    // 3 options ; play, options, exit

    int pointer = 1; 
    

    if (is_pressed('s')){
        pointer ++;

    }
   
    else if (is_pressed('w')){
        pointer --;

    } 
    //clamp pointer
    if (pointer<1){pointer = 1;} 
    if (pointer>menu_options){pointer = menu_options;} 


    if (is_pressed(' ')){
        return pointer;
    }


    //draw menu


    draw_text(screen_w / 2, 50, "SUPER MARIO", 0xFF000FF0, 3);


    switch (pointer){
        case 1:
                draw_text(screen_w / 2, 100, "Play", 0xFFFFFFFF, 2);
                draw_text(screen_w / 2, 125, "Option", 0xFF000FF0, 2);
                draw_text(screen_w / 2, 150, "Exit", 0xFF000FF0, 2);

        case 2:
                draw_text(screen_w / 2, 100, "Play", 0xFF000FF0, 2);
                draw_text(screen_w / 2, 125, "Option", 0xFFFFFFFF, 2);
                draw_text(screen_w / 2, 150, "Exit", 0xFF000FF0, 2);

        case 3:
                draw_text(screen_w / 2, 100, "Play", 0xFF000FF0, 2);
                draw_text(screen_w / 2, 125, "Option", 0xFF000FF0, 2);
                draw_text(screen_w / 2, 150, "Exit", 0xFFFFFFFF, 2);


        default:
                draw_text(screen_w / 2, 100, "Play", 0xFFFFFFFF, 2);
                draw_text(screen_w / 2, 125, "Option", 0xFF000FF0, 2);
                draw_text(screen_w / 2, 150, "Exit", 0xFF000FF0, 2);

    }








    return -1;
}



static void main_loop() { 

    while (1) {
        if (should_update()) {
            screen_clear();
           
           
          menu();

            
            flip(); 
        }
    }
}

void mario_on_start(){
    set_fps(fps);
    main_loop();



}


