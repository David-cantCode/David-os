#ifndef MARIO_H
#define MARIO_H

//needs to be changed cuz player struct is redinfed somewheres else, find way to make structs static?
typedef struct{
    int x, y;

}Vector2;


typedef struct{
    Vector2 position;
}Camera;

typedef struct{
    Vector2 position;
    int is_on_ground;

}Player;


#endif 