
#include "../../../../libary/include/davidgl.h"

extern int mouse_x ;
extern int mouse_y ;

void draw_mouse() {
    draw_line(mouse_x, mouse_y, mouse_x + 5, mouse_y + 10, 0xFFFFFFFF);
    draw_line(mouse_x, mouse_y, mouse_x + 10, mouse_y + 5, 0xFFFFFFFF);
    draw_line(mouse_x + 10, mouse_y + 5, mouse_x + 5, mouse_y + 10, 0xFFFFFFFF);
}


static void main_loop() { 

    while (1) {
        if (should_update()) {
            screen_clear();
           
           
            draw_mouse();

            
            flip(); 
        }
    }
}

void mtest_start(){
    set_fps(60);

    main_loop();



}


