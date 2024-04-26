#include "scene_manager.hpp"

#include <raylib.h>
#include <raymath.h>

#include <iostream>
#include <string>
#include <unordered_map>

class TitleScene : public Scene
{
    Texture raylib_logo;
    Texture rocket;

public:
    UiLibrary uiLibrary;

    void Begin() override
    {
        // raylib_logo = ResourceManager::GetInstance()->GetTexture("Sky_Background.png");
        // rocket = ResourceManager::GetInstance()->GetTexture("rocket_sprite.png");
        SetTargetFPS(TARGET_FPS);
    }

    void End() override {}

    void Update() override
    {

    }

    void Draw() override
    {
        // DrawTexturePro(raylib_logo, {0, 0, 360, 600}, {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT}, {0, 0}, 0.0f, WHITE);
        // DrawTexturePro(rocket, {123, 8, 389, 512}, {(WINDOW_WIDTH / 2) - 20, (WINDOW_HEIGHT / 2) - 30, 60, 78}, {0, 0}, 0.0f, WHITE);
        DrawText("Jemuel the Footsoldier", WINDOW_WIDTH/3 + 30, WINDOW_HEIGHT / 4, 30, BLACK);

        if (uiLibrary.Button(0, "Start Game", { WINDOW_WIDTH/2 - 50, WINDOW_HEIGHT/2 + 70, 100, 40 }) && !isTicked)
        {
            if (GetSceneManager() != nullptr) {
                GetSceneManager()->SwitchScene(1);
            }
        }
        if (uiLibrary.Button(1, "Close", { WINDOW_WIDTH/2 - 50, WINDOW_HEIGHT/2 + 120, 100, 40 }) && !isTicked)
        {
            CloseWindow();
        }
        uiLibrary.Label(2, "Press 'A' to move left", { WINDOW_WIDTH/2 - 70, WINDOW_HEIGHT/2 + 170, 80, 40 });
        uiLibrary.Label(3, "Press 'D' to move right", { WINDOW_WIDTH/2 - 70, WINDOW_HEIGHT/2 + 195, 80, 40 });
        uiLibrary.Label(4, "Press 'W' to jump", { WINDOW_WIDTH/2 - 70, WINDOW_HEIGHT/2 + 220, 80, 40 });
    }
};