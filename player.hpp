#ifndef PLAYER
#define PLAYER

#include <raylib.h>
#include <raymath.h>
#include <vector>

#include "entity.hpp"
#include "levelReader.hpp"

class Player;

class PlayerState{
public:
    virtual ~PlayerState(){}
    virtual void Enter(Player& player) = 0;
    virtual void Update(Player& player, float delta_time) = 0;
};

class PlayerIdle : public PlayerState{
public:
    void Enter(Player& player);
    void Update(Player& player, float delta_time);
};

class PlayerMoving : public PlayerState{
public:
    void Enter(Player& player);
    void Update(Player& player, float delta_time);
};

class PlayerJumping : public PlayerState{
public:
    void Enter(Player& player);
    void Update(Player& player, float delta_time);
};

class PlayerFalling : public PlayerState{
public:
    void Enter(Player& player);
    void Update(Player& player, float delta_time);
};

// class PlayerDodging : public PlayerState{
// public:
//     void Enter(Player& player);
//     void Update(Player& player, float delta_time);
// };

// class PlayerAttacking : public PlayerState{
// public:
//     void Enter(Player& player);
//     void Update(Player& player, float delta_time);
// };

// class PlayerBlocking : public PlayerState{
// public:
//     void Enter(Player& player);
//     void Update(Player& player, float delta_time);
// };

class Player : public Entity{
public:
    Vector2 hitboxPos;
    float hitboxRad;

    Vector2 velocity;
    float timer;
    float invulnerabilityTimer;
    float powerUpTimer;
    float jumpTimer;
    float fallTimer;

    Vector2 wallPos;
    Vector2 wallDim;
    Vector2 screenEdge;
    
    PlayerIdle idle;
    PlayerMoving moving;
    PlayerJumping jumping;
    PlayerFalling falling;
    // PlayerDodging dodging;
    // PlayerAttacking attacking;
    // PlayerBlocking blocking;

    Player();
    Player(Vector2 pos, float rad, float spd, int hp);

    void Update(float delta_time);
    
    void Draw();

    void SetState(PlayerState* new_state);

    bool HandleCollision(Entity* entity);

    // void HandlePlatformCollision(auto entity);

    void HandleBounds(float xBound);

    void HandleTileCollision(int pix, Tile* allTiles, std::vector<std::vector<int>> grid);

private:
    PlayerState* current_state;
};

#endif