
#include "../../libary/include/util.h"
#include "../../libary/include/stdconsole.h"


extern void snake_innit();
extern void pong_on_start();
extern void tetris_main();
extern void mtest_start();
extern void mario_on_start();
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
    else if (compare_string(game, "mtest")==0){
        mtest_start();
    }
    else if (compare_string(game, "mario")==0){
        mario_on_start();
    }




    else{
        print("unknown game\n");
    }


}



void list_games(){


    print("Info: Play game using 'play ...'\n"); 

    print("(1) pong\n"); 
    print("(2) pnake\n"); 
    print("(3) tetris\n");
    print("(4) mario\n");
    print("(5) dkong\n");
}