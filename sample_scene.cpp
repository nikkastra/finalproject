#include <raylib.h>

#include "title_scene.hpp"
#include "game_scene.hpp"
#include "gameover_scene.hpp"

int main() {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Jemuel the Footsoldier");

    SceneManager scene_manager;

    TitleScene title_scene;
    title_scene.SetSceneManager(&scene_manager);

    GameScene game_scene(1);
    game_scene.SetSceneManager(&scene_manager);
    GameScene game_scene1(2);
    game_scene1.SetSceneManager(&scene_manager);
    GameScene game_scene2(3);
    game_scene2.SetSceneManager(&scene_manager);

    GameOverScene gameover_scenew(true);
    gameover_scenew.SetSceneManager(&scene_manager);
    GameOverScene gameover_scenel(false);
    gameover_scenel.SetSceneManager(&scene_manager);

    scene_manager.RegisterScene(&title_scene, 0);
    scene_manager.RegisterScene(&game_scene, 1);
    scene_manager.RegisterScene(&game_scene1, 2);
    scene_manager.RegisterScene(&game_scene2, 3);
    scene_manager.RegisterScene(&gameover_scenew, 4);
    scene_manager.RegisterScene(&gameover_scenel, 5);

    scene_manager.SwitchScene(0);

    while(!WindowShouldClose()) {
        Scene* active_scene = scene_manager.GetActiveScene();

        BeginDrawing();
        ClearBackground({24, 33, 93, 255});

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