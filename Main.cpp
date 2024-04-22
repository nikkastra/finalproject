#include <raylib.h>
#include <raymath.h>
#include <iostream>

#include "PlayerStateMachine.cpp"
#include "EnemyStateMachine.cpp"

#include "levelReader.hpp"
#include "bezier.hpp"

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 704;
const float FPS = 60;
const int PIXELS = 64;

class Entity;

struct Wall{
    Vector2 position;
    Vector2 dimensions;
    Color c;

    void Draw(){
        DrawRectangleV(position, dimensions, c);
    }
};

struct CameraBoundary{
    Vector2 startPos;
    Vector2 endPos;
    Color c;

    void Draw(){
        DrawLineV(startPos, endPos, c);
    }
};

int main(){
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "miss ko na siya");
    SetTargetFPS(FPS);

    readLevel(2);

    //----------------------- BEZIER CURVES -----------------------//
    std::vector<int> coefficients = pascalsTriangleRow(r);

    //curve 1
    for(int i = 0; i < noOfControlPoints; i++){
        circles[i].center = {enemyOne[i].x*PIXELS, enemyOne[i].y*PIXELS};
        circles[i].color = GREEN;
    }

    //curve 2
    for(int i = 0; i < noOfControlPoints; i++){
        circles2[i].center = {enemyTwo[i].x*PIXELS, enemyTwo[i].y*PIXELS};
        circles2[i].color = GREEN;
    }

    //----------------------- BEZIER CURVES -----------------------//

    Camera2D camera_view = {0};
    float EDGE_X[0], EDGE_Y[0];
    camera_view.offset = {WINDOW_WIDTH/3, WINDOW_HEIGHT/2 - 3*PIXELS};
    camera_view.zoom = 1.0f;

    // Texture joshHutcherson = LoadTexture("josh_hutcherson.png");
    Texture tileMap = LoadTexture(tilesetName);

    Player player(playerPos, 50.0f, 300.0f, 5);
    Enemy enemy1(circles[0].center, 100.0f, 100.0f, 2);
    Enemy enemy2(circles2[0].center, 100.0f, 100.0f, 2);

    int pointPointer = 0;
    int pointPointer2 = 1;
    bool reverse = false;
    bool reverse2 = false;

    Wall platform;
    platform.position = {0, WINDOW_HEIGHT - PIXELS};
    platform.dimensions = {PIXELS*8, PIXELS};
    platform.c = BLANK;

    Wall platform2;
    platform2.position = {PIXELS*12, WINDOW_HEIGHT - PIXELS*6};
    platform2.dimensions = {PIXELS*9, PIXELS};
    platform2.c = BLANK;

    CameraBoundary line;
    line.startPos = {WINDOW_WIDTH/3, 0};
    line.endPos = {WINDOW_WIDTH/3, WINDOW_HEIGHT};
    line.c = RED;

    CameraBoundary line2;
    line2.startPos = {WINDOW_WIDTH*3, 0};
    line2.endPos = {WINDOW_WIDTH*3, WINDOW_HEIGHT};
    line2.c = BLANK;

    Vector2 camTarget = {WINDOW_WIDTH/3, WINDOW_HEIGHT/2};

    while(!WindowShouldClose()){
        float delta_time = GetFrameTime();

        camera_view.target = camTarget;

        if(player._position.x >= GetScreenToWorld2D(line.startPos, camera_view).x && line2.startPos.x >= GetScreenToWorld2D({WINDOW_WIDTH, 0}, camera_view).x){
            camTarget.x += player._speed*delta_time;
        }

        player.Update(delta_time);
        if(player._position.y >= WINDOW_HEIGHT + 3*PIXELS){
            std::cout << "gg" << std::endl;
        }
        player.HandleBounds(GetScreenToWorld2D(Vector2Zero(), camera_view).x);
        // player.HandlePlatformCollision(platform);
        player.HandleTileCollision(PIXELS, allTiles, grid);

        //----------------------- BEZIER CURVES -----------------------//
        std::vector<int> coefficients = pascalsTriangleRow(r);

        //curve 1
        actualPoints.clear();
        actualPointsDerivative.clear();

        std::vector<Circle> points;
        for(int j = 0; j <= r; j++){
            points.push_back(circles[j]);
        }
        for(int j = 0; j <= s; j++){
            addPointParametric(points, (float) j/s, coefficients);
        }

        curveDerivative(circles, r-1);

        //curve 2
        actualPoints2.clear();
        actualPointsDerivative2.clear();

        std::vector<Circle> points2;
        for(int j = 0; j <= r; j++){
            points2.push_back(circles2[j]);
        }
        for(int j = 0; j <= s; j++){
            addPointParametric2(points2, (float) j/s, coefficients);
        }

        curveDerivative2(circles2, r-1);

        //----------------------- BEZIER CURVES -----------------------//

        BeginDrawing();
        ClearBackground({24, 33, 93, 255});

        BeginMode2D(camera_view);

        for(int i = 0; i < grid.size(); i++){
            std::vector<int> row = grid[i];
            for(int j = 0; j < row.size(); j++){
                DrawTexturePro(tileMap, allTiles[row[j]].locationInSpriteSheet, {(float) (PIXELS*j), (float) (PIXELS*i), PIXELS, PIXELS}, Vector2Zero(), 0, WHITE);
            }
        }

        // DrawTexture(joshHutcherson, 0, 0, WHITE);
        player.Draw();
        // platform.Draw();

        //----------------------- BEZIER CURVES -----------------------//

        //curve 1
        for(int i = 0; i < noOfControlPoints; i++){
            DrawCircleV(circles[i].center, circles[i].radius, circles[i].color);
        }
        for(auto x = actualPoints.begin(); x != (actualPoints.end()-1); x++){
            DrawLineV((*x).actualPoint, (*(x+1)).actualPoint, BLACK);
        }
        int divisor = actualPoints.size()/v;
        for(int i = 0; i < actualPoints.size()-1; i++){
            if(i % divisor == 0){
                actualPoints[i].tangent = Vector2Subtract(actualPoints[i].actualPoint, actualPointsDerivative[i/divisor]);
                DrawLineV(actualPoints[i].actualPoint, actualPoints[i].tangent, PINK);

                actualPoints[i].normal = Vector2Rotate(Vector2Subtract(actualPoints[i].actualPoint, actualPoints[i].tangent), 3.14/2);
                DrawLineV(actualPoints[i].actualPoint, Vector2Add(actualPoints[i].actualPoint, actualPoints[i].normal), PINK);

                actualPoints[i].hasTangent = true;
            }
        }
        // enemy1 movement
        enemy1.Draw();
        enemy1._position = actualPoints[pointPointer].actualPoint;
        if(actualPoints[pointPointer].hasTangent){
            Vector2 tangent = Vector2Normalize(Vector2Subtract(actualPoints[pointPointer].tangent, actualPoints[pointPointer].actualPoint));
            float cosTheta = Vector2DotProduct(tangent, {1,0});
            enemy1.rotation = acos(cosTheta)*(180/3.14);
            if(tangent.y < 0){
                enemy1.rotation *= -1;
            }
        }
        if(!reverse){
            pointPointer++;
        } else {
            pointPointer--;
        }
        if(pointPointer == actualPoints.size()){
            reverse = true;
        } else if (pointPointer == 0){
            reverse = false;
        }

        //curve 2
        for(int i = 0; i < noOfControlPoints; i++){
            DrawCircleV(circles2[i].center, circles2[i].radius, circles2[i].color);
        }
        for(auto x = actualPoints2.begin(); x != (actualPoints2.end()-1); x++){
            DrawLineV((*x).actualPoint, (*(x+1)).actualPoint, BLACK);
        }
        int divisor2 = actualPoints2.size()/v;
        for(int i = 0; i < actualPoints2.size()-1; i++){
            if(i % divisor2 == 0){
                actualPoints2[i].tangent = Vector2Subtract(actualPoints2[i].actualPoint, actualPointsDerivative2[i/divisor2]);
                DrawLineV(actualPoints2[i].actualPoint, actualPoints2[i].tangent, PINK);

                actualPoints2[i].normal = Vector2Rotate(Vector2Subtract(actualPoints2[i].actualPoint, actualPoints2[i].tangent), 3.14/2);
                DrawLineV(actualPoints2[i].actualPoint, Vector2Add(actualPoints2[i].actualPoint, actualPoints2[i].normal), PINK);

                actualPoints2[i].hasTangent = true;
            }
        }

        //enemy2 movement
        enemy2.Draw();
        enemy2._position = actualPoints2[pointPointer2].actualPoint;
        if(actualPoints2[pointPointer2].hasTangent){
            Vector2 tangent = Vector2Normalize(Vector2Subtract(actualPoints2[pointPointer2].tangent, actualPoints2[pointPointer2].actualPoint));
            float cosTheta = Vector2DotProduct(tangent, {1,0});
            enemy2.rotation = acos(cosTheta)*(180/3.14);
            if(tangent.y < 0){
                enemy2.rotation *= -1;
            }
        }
        if(!reverse2){
            pointPointer2++;
        } else {
            pointPointer2--;
        }
        if(pointPointer2 == actualPoints2.size()){
            reverse2 = true;
        } else if (pointPointer2 == 0){
            reverse2 = false;
        }
        //----------------------- BEZIER CURVES -----------------------//

        EndMode2D();

        // line.Draw();
        EndDrawing();
    }
    CloseWindow();

    return 0;
}