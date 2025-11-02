
//simple 2 player pong game refrenced from my python pong game


#include <stdint.h>
#include "../../libary/include/davidgl.h"
#include "pong.h"
#include "../../libary/include/util.h"



typedef struct {
  int x;
  int y;
} Direction;

//this my first time actually learning structs lmao and i love them
typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    int x;
    int y;

}Velocity;

typedef struct {
    Position pos;        
    Direction dir;
    int score;        
} Player;


typedef struct{
    Position pos;
    Direction dir;
    Velocity velocity;

}Ball;

int player_w = 15;
int player_h = 200;
Player player_1;
Player player_2;
Ball ball;

extern uint32_t screen_width;
extern uint32_t screen_height;

uint32_t white = 0xFFFFFFFF;

//width, height
int bounds[2];


int running;


void draw(){
    draw_rect(player_1.pos.x, player_1.pos.y, 10, 80, 0xFFFFFFFF);

    draw_rect(player_2.pos.x, player_2.pos.y, 10, 80, 0xFFFFFFFF);

    draw_circle(ball.pos.x, ball.pos.y, 6, 0xFFFFFFFF);

    draw_rect(0, screen_height - 2, screen_width , 2, 0xFFFFFFFF);

    //text
    char score_txt[5];
    int_to_str(player_1.score, score_txt);
    draw_text(215, 20, score_txt,0xFFFFFFFF, 2);
    draw_text(332, 20, "-", 0xFFFFFFFF, 2);
    int_to_str(player_2.score, score_txt);
    draw_text(447, 20, score_txt,0xFFFFFFFF, 2 );






}



void controls(){
    int speed = 5; 

    if (is_pressed('w') && player_1.pos.y > 0) {
        player_1.pos.y -= speed;
    }
    if (is_pressed('s') && player_1.pos.y + 80 < bounds[1]) {
        player_1.pos.y += speed;
    }

    if (is_pressed('o') && player_2.pos.y > 0) {
        player_2.pos.y -= speed;
    }
    if (is_pressed('l') && player_2.pos.y + 80 < bounds[1]) {
        player_2.pos.y += speed;
    }

    if (is_pressed('q')){
        running = 0;
    }
}



void reset_ball(){
    ball.pos.x = screen_width /3.15; ball.pos.y = screen_height /2;
    ball.dir.x *= -1; ball.dir.y *= -1;

};

void ball_controller(){
    //if ball touched top/ bottem 
    int ball_r = 15;

    if (ball.pos.y >= bounds[1] || ball.pos.y <= 0 ){
        ball.velocity.y *= -1;
    }

    //if ball touched sides

        //p1
        if (ball.pos.x - ball_r <= player_1.pos.x + player_w &&  
            ball.pos.x >= player_1.pos.x &&                      
            ball.pos.y + ball_r >= player_1.pos.y -100 &&            
            ball.pos.y - ball_r <= player_1.pos.y + player_h + 100) {  
            
            ball.dir.x = 1; 
        }

        //p2
        else if (ball.pos.x + ball_r >= player_2.pos.x &&             
            ball.pos.x <= player_2.pos.x + player_w &&           
            ball.pos.y + ball_r >= player_2.pos.y - 100&&            
            ball.pos.y - ball_r <= player_2.pos.y + player_h +100 ) {  
            
            ball.dir.x = -1; 

        }


        else if (ball.pos.x < player_1.pos.x + player_w){
        player_2.score++;
        reset_ball();
        }

        else if  (ball.pos.x > player_2.pos.x - player_w) {
        player_1.score++;
        reset_ball();
        }




        //ball move:
        ball.pos.x += ball.velocity.x * ball.dir.x;
        ball.pos.y += ball.velocity.y * ball.dir.y;
}



void main_loop() {

    while (running == 1) {
        if (should_update()) {


            screen_clear();
            draw();
            controls();
            ball_controller();

            flip(); 

        }
    }
    screen_clear();
}

void pong_on_start(){
    set_fps(60); 
    player_1.pos.x = 8; player_1.pos.y = screen_height /2;
    player_1.score = 0;
    player_2.pos.x = screen_width /1.65 ; player_2.pos.y = screen_height /2;
    player_2.score = 0;
    ball.pos.x = screen_width /3.15; ball.pos.y = screen_height /2;
    ball.velocity.x = 3;ball.velocity.y = 3;
    ball.dir.x = 1; ball.dir.y = 1;

    bounds[0] = screen_width / 1.65 ;
    bounds[1] = screen_height;
    running = 1;

    main_loop();
    
}
