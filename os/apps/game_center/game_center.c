
#include "../../libary/include/util.h"
#include "../../libary/include/stdconsole.h"


extern void snake_innit();
extern void pong_on_start();
extern void tetris_main();
void play_game(char* game){


    if (compare_string(game, "snake")==0){
        snake_innit();
    }
    else if (compare_string(game, "pong")==0){
        pong_on_start();
    }
    else if (compare_string(game, "tetris")==0){
        tetris_main();
    }


}



void list_games(){



    print("(1) Pong\n"); 
    print("(2) Snake\n"); 
    print("(3) Tetris\n");
}