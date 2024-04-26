#include <raylib.h>
#include <raymath.h>
#include <string>
#include <iostream>

#include "Player.hpp"
#include "entity.hpp"

class Entity;

void Player::Update(float delta_time){
    if(_damaged){
        invulnerabilityTimer += delta_time;
        if(invulnerabilityTimer > 2.0f){
            _damaged = false;
            invulnerabilityTimer = 0;
        }
    }

    // for the speed buff
    if(_buffed){
        _buffTimer += delta_time;
        if(_buffTimer  > 5.0f){
            _buffed = false;
            _buffTimer = 0.0f;
            _speedMultiplier = 1.0f;
        }
    }

    current_state->Update(*this, delta_time);
}

void Player::Draw(){
    DrawCircleV(_position, _radius, _color);
    // if(current_state == &attacking){
        // DrawCircleV(hitboxPos, hitboxRad, _color);
    // }
}

void Player::SetState(PlayerState* new_state){
    current_state = new_state;
    current_state->Enter(*this);
}

bool Player::HandleCollision(Entity* entity){
    Vector2 q;

    // if(current_state == &attacking){
    //     if(hitboxPos.x < entity->_position.x){
    //         q.x = entity->_position.x;
    //     } else if (hitboxPos.x > entity->_position.x + entity->_size){
    //         q.x = entity->_position.x + entity->_size;
    //     } else {
    //         q.x = hitboxPos.x;
    //     }

    //     if(hitboxPos.y < entity->_position.y){
    //         q.y = entity->_position.y;
    //     } else if (hitboxPos.y > entity->_position.y + entity->_size){
    //         q.y = entity->_position.y + entity->_size;
    //     } else {
    //         q.y = hitboxPos.y;
    //     }

    //     if(Vector2Distance(hitboxPos, q) <= _radius && !entity->_damaged){
    //         entity->_healthPoints -= 1;
    //         entity->_damaged = true;
    //     }
    // } else {
        entity->_damaged = false;

        if(_position.x < entity->_position.x){
            q.x = entity->_position.x;
        } else if (_position.x > entity->_position.x + entity->_size/2){
            q.x = entity->_position.x + entity->_size/2;
        } else {
            q.x = _position.x;
        }

        if(_position.y < entity->_position.y){
            q.y = entity->_position.y;
        } else if (_position.y > entity->_position.y + entity->_size/2){
            q.y = entity->_position.y + entity->_size/2;
        } else {
            q.y = _position.y;
        }

        if(Vector2Distance(_position, q) <= _radius){
            // if(current_state == &dodging){
            //     _healthPoints -= 0;
            // } else if (current_state == &blocking){
            //     _healthPoints -= (int) entity->_damage/2;
            // } else {
                // _healthPoints -= (int) entity->_damage;
            // }
            return true;
        }
    // }
    return false;
}

// void Player::HandlePlatformCollision(auto entity){
//     wallPos = entity.position;
//     wallDim = entity.dimensions;

//     bool malapitNangUmumpog = ((_position.y + _radius) <= entity.position.y + 20.0f) && ((_position.y + _radius) >= entity.position.y - 20.0f);
//     bool playerBoundsInsidePlatformBounds = (_position.x >= entity.position.x) && (_position.x <= entity.position.x + entity.dimensions.x);
//     bool naumpog = (_position.y + _radius) >= entity.position.y;

//     Vector2 collision_normal = Vector2Subtract(_position, entity.position);
//     float normal_rel_vel_dot = Vector2DotProduct(collision_normal, velocity);

//     if (malapitNangUmumpog && (playerBoundsInsidePlatformBounds)){
//         isColliding = true;
//     } else { 
//         isColliding = false;
//     }
// }

void Player::HandleBounds(float xBound){
    if(_position.x - _radius <= xBound){
        _position.x = xBound + _radius;
    }
}

void Player::HandleTileCollision(int pix, Tile* allTiles, std::vector<std::vector<int>> grid){
    int tileXPos = (int) hitboxPos.x/pix;
    int tileYPos = (int) hitboxPos.y/pix;
    // targetTile = {(float) tileXPos, tileYPos};

    if(allTiles[grid[tileYPos][tileXPos]].has_collision){
        wallPos = {(float) tileXPos*pix, (float) tileYPos*pix};
        isCollidingDown = true;
    } else {
        isCollidingDown = false;
    }
}

//Player::Player(Vector2 pos, float rad, float spd, int hp)
Player::Player(Vector2 pos, float rad, float spd, int hp){
    // Entity Based Variables
    _position = pos;
    _radius = rad;
    _speed = spd;
    _buffTimer = 0;
    _speedMultiplier = 1.0f;
    _buffed = false;
    _damaged = false;
    _healthPoints = hp;
    isCollidingDown = false;
    
    // Player Based Variables
    timer = 0;
    invulnerabilityTimer = 0;
    hitboxPos = {_position.x + _radius +25.0f, _position.y};
    hitboxRad = _radius/2;

    SetState(&falling);
    
}


void PlayerIdle::Enter(Player& player){
    player._color = PINK;
}

void PlayerMoving::Enter(Player& player){
    player._color = GREEN;
}

void PlayerJumping::Enter(Player& player){
    player._color = ORANGE;
    player.jumpTimer = 0.5f;
}

void PlayerFalling::Enter(Player& player){
    player._color = BLUE;
    player.fallTimer = 0.0f;
}

// void PlayerDodging::Enter(Player& player){
//     player._color = ORANGE;
// }

// void PlayerAttacking::Enter(Player& player){
//     player._color = BROWN;
// }

// void PlayerBlocking::Enter(Player& player){
//     player._color = BLUE;
// }

void PlayerIdle::Update(Player& player, float delta_time){
    if(IsKeyDown(KEY_A) || IsKeyDown(KEY_D)){
        player.SetState(&player.moving);
    }
    if(IsKeyDown(KEY_W)){
        player.SetState(&player.jumping);
    }
    if(!player.isCollidingDown){
        player.SetState(&player.falling);
    }
    // if(IsMouseButtonDown(0)) {
    //     player.SetState(&player.attacking);
    // }
    // if(IsMouseButtonDown(1)) {
    //     player.SetState(&player.blocking);
    // }
}

void PlayerMoving::Update(Player& player, float delta_time){
    player.velocity = Vector2Zero();

    if(IsKeyDown(KEY_A)){
        player.velocity.x = -1*(player._speed * player._speedMultiplier);
    }
    if(IsKeyDown(KEY_D)){
        player.velocity.x = (player._speed * player._speedMultiplier);
    }
    if(IsKeyDown(KEY_W)){
        player.SetState(&player.jumping);
    }
    if(!player.isCollidingDown){
        player.SetState(&player.falling);
    }
    // if(!player.isColliding && IsKeyPressed(KEY_SPACE)){
    //     player.SetState(&player.dodging);
    // }

    // if(player.isCollidingLeft){
    //     player._position.x = player.wallPosLeft.x + player.wallDim.x + player._radius;
    // } else if(player.isCollidingRight){
    //     player._position.x = player.wallPosRight.x - 1.5f*player._radius;
    // } else {
        player._position = Vector2Add(player._position, Vector2Scale(player.velocity, delta_time));
    // } 

    // if(IsMouseButtonDown(0)) {
    //     player.SetState(&player.attacking);
    // }

    if(Vector2Length(player.velocity) == 0) {
        player.SetState(&player.idle);
    }
    else {
        player.hitboxPos = {player._position.x, player._position.y + 1.5f*player._radius};
    }
}

void PlayerJumping::Update(Player& player, float delta_time){
    if(IsKeyDown(KEY_A)){
        player.velocity.x = -1*(player._speed * player._speedMultiplier);
    }
    if(IsKeyDown(KEY_D)){
        player.velocity.x = (player._speed * player._speedMultiplier);
    }

    if(player.jumpTimer > 0){
        player.velocity.y = -1*player._speed*10*player.jumpTimer;
        player.jumpTimer -= delta_time;
    } else {
        player.SetState(&player.falling);
    }
    player._position = Vector2Add(player._position, Vector2Scale(player.velocity, delta_time));
    player.hitboxPos = {player._position.x, player._position.y + 1.5f*player._radius};
}

void PlayerFalling::Update(Player& player, float delta_time){
    if(IsKeyDown(KEY_A)){
        player.velocity.x = -1*(player._speed * player._speedMultiplier);
    }
    if(IsKeyDown(KEY_D)){
        player.velocity.x = (player._speed * player._speedMultiplier);
    }

    if(player.isCollidingDown){
        player._position.y = player.wallPos.y - player._radius;
        player.velocity = Vector2Zero();
        player.SetState(&player.idle);
    } else {
        player.velocity.y = player._speed*10*player.fallTimer;
        player.fallTimer += delta_time;
    }
    player._position = Vector2Add(player._position, Vector2Scale(player.velocity, delta_time));
    player.hitboxPos = {player._position.x, player._position.y + 1.5f*player._radius};
}

// void PlayerDodging::Update(Player& player, float delta_time){
//     if(player.isColliding){
//         player._position =  Vector2Add(player._position, Vector2Zero());
//     } else {
//         player._position = Vector2Add(player._position, Vector2Scale(Vector2Scale(player.velocity, delta_time), 10.0f));
//         player.hitboxPos = {player._position.x + player.velocity.x/(player._speed * player._speedMultiplier)*(player._radius + 25.0f), player._position.y + player.velocity.y/(player._speed * player._speedMultiplier)*(player._radius + 25.0f)};
//     }
//     player.timer += delta_time;
//     if(player.timer > 0.125){
//         player.timer = 0;
//         player.SetState(&player.idle);
//     }
// }

// void PlayerAttacking::Update(Player& player, float delta_time){
//     player.timer += delta_time;
//     if(player.timer > 0.25){
//         player.timer = 0;
//         player.SetState(&player.idle);
//     }
// }

// void PlayerBlocking::Update(Player& player, float delta_time){
//     if(IsMouseButtonReleased(1)){
//         player.SetState(&player.idle);
//     }
// }