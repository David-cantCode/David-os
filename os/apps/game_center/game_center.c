
#include "../../libary/include/util.h"


extern void snake_innit();
extern void pong_on_start();

void play_game(char* game){


    if (compare_string(game, "snake")==0){
        snake_innit();
    }
    else if (compare_string(game, "pong")==0){
        pong_on_start();
    }


}