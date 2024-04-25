#ifndef WALL
#define WALL

#include <raylib.h>
#include <raymath.h>

struct Wall{
    Vector2 position;
    Vector2 dimensions;
    Color c;

    void Draw(){
        DrawRectangleV(position, dimensions, c);
    }
};

#endif