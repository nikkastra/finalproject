#pragma once

#ifndef SCENE_MANAGER
#define SCENE_MANAGER

#include <raylib.h>
#include <raymath.h>

#include <iostream>
#include <string>
#include <unordered_map>

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 704;
const float TARGET_FPS = 60;

bool isTicked = false;

float score;
float high_score = -10.0f; 

// Struct to encapsulate our UI library
struct UiLibrary
{
    // ID of the currently hot widget
    int hot = -1;

    // ID of the currently active widget
    int active = -1;

    // Creates a button with the specified text and bounds
    // Returns true if this button was clicked in this frame
    bool Button(int id, const std::string& text, const Rectangle& bounds)
    {
        bool result = false;

        // If this button is the currently active widget, that means
        // the user is currently interacting with this widget
        if (id == active)
        {
            // If the user released the mouse button while we are active,
            // register as a click
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            {
                // Make sure that we are still the hot widget
                // (User might have dragged mouse to another widget, in which case we will not count as a button click)
                if (id == hot)
                {
                    result = true;
                }

                // Set ourselves to not be active anymore
                active = -1;
            }
        }

        // If we are currently the hot widget
        if (id == hot)
        {
            // If the user pressed the left mouse button, that means the user started
            // interacting with this widget, so we set this widget as active
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                active = id;
            }
        }

        // If the mouse cursor is hovering within our boundaries
        if (CheckCollisionPointRec(GetMousePosition(), bounds))
        {
            // Set this widget to be the hot widget
            hot = id;
        }
        // If the mouse cursor is not on top of this widget, and this widget
        // was previously the hot widget, set the hot widget to -1
        // We check if this widget was the previously hot widget since there is a possibility that
        // the hot widget is now a different widget, and we don't want to overwrite that with -1
        else if (hot == id)
        {
            hot = -1;
        }

        // Draw our button regardless of what happens
        if(active == id){
            DrawRectangleRec(bounds, BLUE);
            DrawText(text.c_str(), bounds.x, bounds.y, 14, YELLOW);
        } else if (hot == id) {
            DrawRectangleRec(bounds, GREEN);
            DrawText(text.c_str(), bounds.x, bounds.y, 14, PINK);
        } else {
            DrawRectangleRec(bounds, GRAY);
            DrawText(text.c_str(), bounds.x, bounds.y, 14, BLACK);
        }

        return result;
    }

    void Label(int id, const std::string& text, const Rectangle& bounds){
        DrawText(text.c_str(), bounds.x, bounds.y, 14, BLACK);
    }
};

class SceneManager;

// Base class that all scenes inherit
class Scene
{
    // Reference to the scene manager.
    // In practice, you would want to make this private (or protected)
    // and set this via the constructor.
    SceneManager *scene_manager;

public:
    // Begins the scene. This is where you can load resources
    virtual void Begin() = 0;

    // Ends the scene. This is where you can unload resources
    virtual void End() = 0;

    // Updates scene's state (physics, input, etc. can be added here)
    virtual void Update() = 0;

    // Draws the scene's current state
    virtual void Draw() = 0;

    void SetSceneManager(SceneManager *scene_manager)
    {
        this->scene_manager = scene_manager;
    }

    SceneManager *GetSceneManager()
    {
        return scene_manager;
    }
};

class SceneManager
{
    // Mapping between a scene ID and a reference to the scene
    std::unordered_map<int, Scene *> scenes;

    // Current active scene
    Scene *active_scene = nullptr;

public:
    // Adds the specified scene to the scene manager, and assigns it
    // to the specified scene ID
    void RegisterScene(Scene *scene, int scene_id)
    {
        scenes[scene_id] = scene;
    }

    // Removes the scene identified by the specified scene ID
    // from the scene manager
    void UnregisterScene(int scene_id)
    {
        scenes.erase(scene_id);
    }

    // Switches to the scene identified by the specified scene ID.
    void SwitchScene(int scene_id)
    {
        // If the scene ID does not exist in our records,
        // don't do anything (or you can print an error message).
        if (scenes.find(scene_id) == scenes.end())
        {
            std::cout << "Scene ID not found" << std::endl;
            return;
        }

        // If there is already an active scene, end it first
        if (active_scene != nullptr)
        {
            active_scene->End();
        }

        std::cout << "Moved to Scene " << scene_id << std::endl;

        active_scene = scenes[scene_id];

        active_scene->Begin();
    }

    // Gets the active scene
    Scene *GetActiveScene()
    {
        return active_scene;
    }
};

// Resource manager implemented as a singleton
class ResourceManager
{
    std::unordered_map<std::string, Texture> textures;

    ResourceManager() {}

public:
    // Delete copy constructor and copy operator (=)
    // Ensures there will only be one instance of the resource manager
    ResourceManager(const ResourceManager &) = delete;
    void operator=(const ResourceManager &) = delete;

    static ResourceManager *GetInstance()
    {
        static ResourceManager insance;
        return &insance;
    }

    Texture GetTexture(const std::string &path)
    {
        // If the texture does not exist yet in our records,
        // load it and store it in memory.
        if (textures.find(path) == textures.end())
        {
            std::cout << "Loaded " << path << " from Disk" << std::endl;
            textures[path] = LoadTexture(path.c_str());
        }
        else
        {
            std::cout << "Resource Already Loaded" << std::endl;
        }

        return textures[path];
    }

    // Used for unloading all the textures when the game is closed.
    void UnloadAllTextures()
    {
        for (auto it : textures)
        {
            UnloadTexture(it.second);
        }

        textures.clear();
    }
};

#endif