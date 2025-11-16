#include "../../libary/include/davidgl.h"
#include "../../libary/include/stdmath.h"
#include "cube.h"
static int screen_w = 600;
static int screen_h = 600;

#define CUBE_SIZE 1.0f
#define ROT_SPEED 0.05f
#define CAMERA_DIST 4.0f

typedef struct { float x, y, z; } Vertex;
typedef struct { int a, b; } Edge;

Vertex cube_vertices[8] = {
    {-CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE},
    { CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE},
    { CUBE_SIZE,  CUBE_SIZE, -CUBE_SIZE},
    {-CUBE_SIZE,  CUBE_SIZE, -CUBE_SIZE},
    {-CUBE_SIZE, -CUBE_SIZE,  CUBE_SIZE},
    { CUBE_SIZE, -CUBE_SIZE,  CUBE_SIZE},
    { CUBE_SIZE,  CUBE_SIZE,  CUBE_SIZE},
    {-CUBE_SIZE,  CUBE_SIZE,  CUBE_SIZE}
};


Edge cube_edges[12] = {
    {0,1},{1,2},{2,3},{3,0},
    {4,5},{5,6},{6,7},{7,4},
    {0,4},{1,5},{2,6},{3,7}
};



void rotate_y(Vertex* v, float angle) {
    float x = v->x;
    float z = v->z;
    v->x = x * m_cos(angle) - z * m_sin(angle);
    v->z = x * m_sin(angle) + z * m_cos(angle);
}

void rotate_x(Vertex* v, float angle) {
    float y = v->y;
    float z = v->z;
    v->y = y * m_cos(angle) - z * m_sin(angle);
    v->z = y * m_sin(angle) + z * m_cos(angle);
}


void project_vertex(Vertex v, int* x, int* y) {
    float scale = 200.0f; 
    *x = (int)((v.x / (v.z + CAMERA_DIST)) * scale + screen_w / 2);
    *y = (int)((v.y / (v.z + CAMERA_DIST)) * scale + screen_h / 2);
}


void draw_cube(Vertex* vertices, Edge* edges, int edge_count) {
    for (int i = 0; i < edge_count; i++) {
        int x0, y0, x1, y1;
        project_vertex(vertices[edges[i].a], &x0, &y0);
        project_vertex(vertices[edges[i].b], &x1, &y1);
        draw_line(x0, y0, x1, y1, 0xFFFFFFFF); 
    }
}

void cube_main() {
    set_fps(30);

    Vertex transformed[8];

    while (1) {
        if (should_update()) {
        
            for (int i = 0; i < 8; i++) transformed[i] = cube_vertices[i];

           
            static float angle = 0.0f;
            angle += ROT_SPEED;
            for (int i = 0; i < 8; i++) {
                rotate_x(&transformed[i], angle * 0.5f);
                rotate_y(&transformed[i], angle);
            }

            
            screen_clear();
            draw_cube(transformed, cube_edges, 12);
            flip();



            if (is_pressed('q')){
                return;
            }
        }
    }
}
