#include <raylib.h>

#include "title_scene.hpp"
#include "game_scene.hpp"

int main() {
    InitWindow(360, 600, "Jemuel's Rocket");

    SceneManager scene_manager;

    TitleScene title_scene;
    title_scene.SetSceneManager(&scene_manager);

    GameScene game_scene;
    game_scene.SetSceneManager(&scene_manager);

    scene_manager.RegisterScene(&title_scene, 0);
    scene_manager.RegisterScene(&game_scene, 1);

    scene_manager.SwitchScene(0);

    while(!WindowShouldClose()) {
        Scene* active_scene = scene_manager.GetActiveScene();

        BeginDrawing();
        ClearBackground(WHITE);

        if (active_scene != nullptr) {
            active_scene->Update();
            active_scene->Draw();
        }

        EndDrawing();
    }

    Scene* active_scene = scene_manager.GetActiveScene();
    if (active_scene != nullptr) {
        active_scene->End();
    }

    ResourceManager::GetInstance()->UnloadAllTextures();

    CloseWindow();
    return 0;
}