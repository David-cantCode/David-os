


//enums are so sexy why didnt i know about this earlier
typedef enum {
    up,
    down,
    left,
    right
} Direction;

//this my first time actually learning structs lmao and i love them
typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    Position pos;        
    Direction dir;        
} Player;


typedef struct{
    Position pos;
    Direction dir;

}Ball;


Player player_1;
Player player_2;
Ball ball;

int screen_height = 800;
int screen_width = 600;



void on_start(){
    player_1.pos.x = 8; player_1.pos.y = screen_height /2;

    player_2.pos.x = screen_width - 8; player_2.pos.y = screen_height /2;

    ball.pos.x = screen_width /2; ball.pos.y = screen_height /2;



}



void draw(){


    
}

void main_loop(){



}