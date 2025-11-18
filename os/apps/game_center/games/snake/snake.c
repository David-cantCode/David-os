#include "davidgl.h"
#include "snake.h"



#define CELL_SIZE 20
#define MAX_SNAKE 1024

typedef enum { UP, DOWN, LEFT, RIGHT } Direction;

typedef struct {
    int x, y;
} Vector;

static Vector snake[MAX_SNAKE];
static int snake_len = 2;
static Direction dir = RIGHT;
static Vector food;

static int game_over = 0;

uint32_t green = 0xFF000000; 
uint32_t red = 0xF8000000;  

//not using global screen_height/width cuz it gave sone issues gotta check that out later
static int screen_w = 600;
static int screen_h = 600;


static uint32_t rng_seed = 12345; 
uint32_t rand_next() {
    rng_seed = rng_seed * 1664525 + 1013904223; 
    return rng_seed;
}



int collision(int x, int y) {
    for (int i = 0; i < snake_len; i++)
        if (snake[i].x == x && snake[i].y == y)
            return 1;
    return 0;
}


void spawn_food() {
    int cols = screen_w / CELL_SIZE;
    int rows = screen_h / CELL_SIZE;

    food.x = (rand_next() % cols) * CELL_SIZE;
    food.y = (rand_next() % rows) * CELL_SIZE;

    while (collision(food.x, food.y)) {
        food.x = (rand_next() % cols) * CELL_SIZE;
        food.y = (rand_next() % rows) * CELL_SIZE;
    }
}

void update_snake() {
    //move body
    for (int i = snake_len - 1; i > 0; i--)
        snake[i] = snake[i - 1];

  
    //move head
    switch (dir) {
        case UP:    snake[0].y -= CELL_SIZE; break;
        case DOWN:  snake[0].y += CELL_SIZE; break;
        case LEFT:  snake[0].x -= CELL_SIZE; break;
        case RIGHT: snake[0].x += CELL_SIZE; break;
    }

    //wrap around screen
    if (snake[0].x < 0) snake[0].x = screen_w - CELL_SIZE;
    if (snake[0].x >= screen_w) snake[0].x = 0;
    if (snake[0].y < 0) snake[0].y = screen_h - CELL_SIZE;
    if (snake[0].y >= screen_h) snake[0].y = 0;

    


    //self col
    for (int i = 1; i < snake_len; i++)
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
            game_over = 1;

    //eat
    if (snake[0].x == food.x && snake[0].y == food.y) {
        if (snake_len < MAX_SNAKE) snake_len++;


        //spawn food in a free spot with 1000 attemps max
        int attempts = 0;
        do {
            spawn_food();
            attempts++;
        } while (collision(food.x, food.y) && attempts < 1000); 
    
    }
}



void draw_snake() {
    for (int i = 0; i < snake_len; i++)
        draw_rect(snake[i].x, snake[i].y, CELL_SIZE, CELL_SIZE, green);
}

void draw_food() {
    draw_rect(food.x, food.y, CELL_SIZE, CELL_SIZE, red);
}

void handle_input() {
    if (is_pressed('w') && dir != DOWN) dir = UP;
    if (is_pressed('s') && dir != UP) dir = DOWN;
    if (is_pressed('a') && dir != RIGHT) dir = LEFT;
    if (is_pressed('d') && dir != LEFT) dir = RIGHT;
    if (is_pressed('q')) game_over = 1; // quit
}

void snake_main() {
    set_fps(10);
    spawn_food();

    //ini snake
    for (int i = 0; i < snake_len; i++) {
        snake[i].x = (snake_len - i - 1) * CELL_SIZE;
        snake[i].y = 0;
    }

    while (!game_over) {
        if (should_update()) {
            handle_input();
            update_snake();

            screen_clear();
            draw_snake();
            draw_food();
            flip();
        }
    }

    screen_clear();
    draw_text(50,50, "you lost", 0xFFFFFFFF, 2);
    flip();
}


void snake_innit(){
    game_over = 0;
    snake_len = 2;


    snake_main();


}
