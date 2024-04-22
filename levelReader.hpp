#ifndef LEVELREADER
#define LEVELREADER

#include <iostream>
#include <raylib.h>
#include <vector>
#include <string>

struct Tile{
    Rectangle locationInSpriteSheet;
    bool has_collision;
};

Tile* allTiles = new Tile[20];
int notWallTiles[] = {0};
std::vector<std::vector<int>> grid;

bool isNotWallTile(int i){
    for(auto x : notWallTiles){
        if(i == x) return true;
    }
    return false;
}

char tilesetName[20];
Vector2 playerPos;
Vector2 enemyOne[4];
Vector2 enemyTwo[4];

char level1[11] = "level1.txt";
char level2[11] = "level2.txt";
char level3[11] = "level3.txt";
char *levelNames[3] = {level1, level2, level3};

void readLevel(int index){
    //--------------DRAW TILESET ALGORITHM----------------//
    // char tilesetName[20];
    int tileCount;
    int tileIndex = 0;
    int gridSizeX = -1;
    int gridSizeY = -1;

    FILE* configFile = fopen(levelNames[index], "r");
    if (configFile) {
        fscanf(configFile, "IMAGE_NAME %s\n", &tilesetName);
        int px, py, e11x, e12x, e13x, e14x, e21x, e22x, e23x, e24x, e11y, e12y, e13y, e14y, e21y, e22y, e23y, e24y;
        fscanf(configFile, "PLAYER_POS %i %i\n", &px, &py);
        playerPos = {(float) px, (float) py};
        fscanf(configFile, "ENEMY_ONE_BEZIER_POINTS %i %i %i %i %i %i %i %i\n", &e11x, &e11y, &e12x, &e12y, &e13x, &e13y, &e14x, &e14y);
        enemyOne[0] = {(float) e11x, (float) e11y,};
        enemyOne[1] = {(float) e12x, (float) e12y,};
        enemyOne[2] = {(float) e13x, (float) e13y,};
        enemyOne[3] = {(float) e14x, (float) e14y,};
        fscanf(configFile, "ENEMY_TWO_BEZIER_POINTS %i %i %i %i %i %i %i %i\n", &e21x, &e21y, &e22x, &e22y, &e23x, &e23y, &e24x, &e24y);
        enemyTwo[0] = {(float) e21x, (float) e21y,};
        enemyTwo[1] = {(float) e22x, (float) e22y,};
        enemyTwo[2] = {(float) e23x, (float) e23y,};
        enemyTwo[3] = {(float) e24x, (float) e24y,};
        fscanf(configFile, "TILE_COUNT %i\n", &tileCount);
        
        int h,w,x,y;
        for(int i = 0; i < tileCount; i++){
            fscanf(configFile, "%i %i %i %i\n", &x, &y, &w, &h);
            allTiles[i].locationInSpriteSheet.x = x;
            allTiles[i].locationInSpriteSheet.y = y;
            allTiles[i].locationInSpriteSheet.width = w;
            allTiles[i].locationInSpriteSheet.height = h;
            if(isNotWallTile(i)){
                allTiles[i].has_collision = false;
            } else {
                allTiles[i].has_collision = true;
            }
        }
        fscanf(configFile, "GRID %i %i\n", &gridSizeX, &gridSizeY);
        for(int i = 0; i < gridSizeY; i++){
            std::vector<int> row;
            for(int j = 0; j < gridSizeX; j++){
                int g;
                fscanf(configFile, "%i", &g);
                row.push_back(g);
            }
            grid.push_back(row);
        }
        fclose(configFile);
        std::cout << "Config file SUCCESS\n";
    } else {
    std::cerr << "Error";
    }
}

#endif