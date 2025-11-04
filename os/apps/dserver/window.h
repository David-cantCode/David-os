#ifndef WINDOW_H
#define WINDOW_H

typedef struct Window {
    int x, y, width, height;
    int color;
    struct Program* program; // pointer to Program
} Window;

#endif
