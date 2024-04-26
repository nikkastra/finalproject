#include "scene_manager.hpp"

#include <raylib.h>
#include <raymath.h>

#include <iostream>
#include <string>
#include <unordered_map>

class GameOverScene : public Scene
{
    Texture gameOverBackground;
    Texture gameOverBackgroundWin;

    public:
    UiLibrary uiLibrary;
    bool win;

    GameOverScene(bool tarantado){
        win = tarantado;
    }

    void Begin() override
    {
        gameOverBackground = ResourceManager::GetInstance()->GetTexture("josh_hutcherson.png");
        gameOverBackgroundWin = ResourceManager::GetInstance()->GetTexture("philip-castillo-lol-jhin-vs-camille-awake-wallpaper-4k-2.jpg");
        SetTargetFPS(TARGET_FPS);
    }

    void End() override
    {

    }

    void Update() override
    {

    }

    void Draw() override
    {
        if(win){
            DrawTexturePro(gameOverBackgroundWin, {0, 0, 1200, 628}, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}, {0, 0}, 0.0f, WHITE);
            // DrawTextureV(gameOverBackground, Vector2Zero(), WHITE);
            DrawText("You Win!", WINDOW_WIDTH / 2 - 75, WINDOW_HEIGHT / 4, 30, GREEN);
        } else {
            DrawTexturePro(gameOverBackground, {0, 0, 1200, 628}, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}, {0, 0}, 0.0f, WHITE);
            // DrawTextureV(gameOverBackground, Vector2Zero(), WHITE);
            DrawText("Game Over", WINDOW_WIDTH / 2 - 75, WINDOW_HEIGHT / 4, 30, RED);
        }

        if (uiLibrary.Button(0, "Retry", {WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2 + 70, 100, 40}) && !isTicked)
        {
            if (GetSceneManager() != nullptr)
            {
                GetSceneManager()->SwitchScene(0); // Switch to the TitleScene or the scene you want to start from.
            }
        }
        if (uiLibrary.Button(1, "Quit", {WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2 + 120, 100, 40}) && !isTicked)
        {
            CloseWindow();
        }
    }
};
