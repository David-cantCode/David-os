

#include <stdint.h>
#include "../../libary/include/davidgl.h"
#include "../../libary/include/stdmath.h"



int r_running;
int map[10][10] = {
    {0,0,1,1,1,1,1,1,1,1},
    {0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,0,1,1,0,0,1},
    {1,0,1,0,0,0,1,0,0,1},
    {1,0,1,0,1,0,1,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,1,1,1,1,0,1},
    {1,0,0,0,0,0,0,1,0,1},
    {1,0,0,1,1,1,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1}
};
const int rows = 10;
const int cols = 10;
const int TILE_SIZE = 100;
double projection_plane_dist;


typedef struct{
    int player_x;
    int player_y;
    int new_x;
    int new_y;

    double angle;
    int speed;
    double rot_speed;

    int ray_length;
    int max_depth;
    int num_rays;
    float fov;
}Player;

Player player;


extern uint32_t screen_width;
extern uint32_t screen_height;


void draw_walls(double distance, double ray_idx){
    if (distance < 0.0001) distance = 0.0001; // avoid dividing by zero 

    double wall_height = ((double)TILE_SIZE / distance) * projection_plane_dist; 

    double x = ray_idx * ((double)screen_width / player.num_rays);
    double y1 = (double)screen_height / 2.0 - wall_height / 2.0;
    double y2 = (double)screen_height / 2.0 + wall_height / 2.0;
    double col_w = (double)screen_width / player.num_rays + 1.0;



    distance /= 4; int shader = 255 - distance ; if (shader > 255){shader = 255;} //CLAMP THE SHADER WOOOOO 

   draw_rect(x, y1, col_w, y2 - y1, 0xFFFFFFFF);

}


void draw_player(){

   draw_rect(player.player_x, player.player_y, 40, 40, 0xFFFFFFFF);

}

void cast_rays(){
    double start_angle = player.angle - player.fov / 2;

    for (double ray = 0; ray < player.num_rays; ray ++) {
        double ray_angle = start_angle + (ray / player.num_rays) * player.fov; 
        double sin_a = m_sin(ray_angle);
        double cos_a = m_cos(ray_angle);


        for (float depth = 0; depth < player.max_depth; depth += 1){
            int ray_x = (int)(player.player_x + cos_a * depth);
            int ray_y = (int)(player.player_y + sin_a * depth);


            int map_x = ray_x / TILE_SIZE;
            int map_y = ray_y / TILE_SIZE;

            if (map_x < 0 || map_x >= cols || map_y < 0 || map_y >= rows)
                break;

            

    


            if (map[map_y][map_x] == 1){ 

    

                int dx = ray_x - player.player_x;
                int dy = ray_y - player.player_y;

                int distance = m_sqrt(dx * dx + dy * dy);
                draw_walls(distance, ray);
          


                break; 
            }


        }

    
}}

void r_controls(){


    if (is_pressed('w'))  {
            player.new_x += m_cos(player.angle) * player.speed;
            player.new_y += m_sin(player.angle) * player.speed;
    }

    if (is_pressed('s'))  {
            player.new_x -= m_cos(player.angle) * player.speed;
            player.new_y -= m_sin(player.angle) * player.speed;
    }




    if (is_pressed('d')) {
            player.angle -= player.rot_speed;
    }

    if (is_pressed('a') ) {
            player.angle += player.rot_speed;
    }


        player.player_x = player.new_x;
    player.player_y = player.new_y;


}


void r_main_loop() {
    r_running = 1;
    while (r_running == 1) {
    if (should_update()) {
        screen_clear();


        draw_player();
        cast_rays();
        r_controls();
       
    

        flip(); 

        }
    }
    screen_clear();
    flip(); 

}


void raycastengine_init(){
    player.angle=0;player.player_x=100;player.player_y=100;
    player.speed=10;player.rot_speed=0.20;player.ray_length=1000;
    player.fov = 3.13 /3;player.num_rays=400;player.max_depth=1000;


    projection_plane_dist = (screen_width / 2.0) / m_tan(player.fov / 2.0);

    r_main_loop();

}