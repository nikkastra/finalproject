#include <raylib.h>
#include <raymath.h>
#include <string>
#include <iostream>

#include "item.hpp"
#include "entity.hpp"

class Entity; 

void Item::Update(float delta_time){
    current_state->Update(*this, delta_time);
}

void Item::Draw(){
    // Rectangle e = {_position.x, _position.y, _size, _size};
    DrawRectangleV(_position, {_size, _size}, ORANGE);
}

void Item::SetState(ItemState* new_state){
    current_state = new_state;
    current_state->Enter(*this);
}

bool Item::HandleCollision(Entity* entity,  float delta_time){ // need delta_time for a timer
    Vector2 q;
    
    // for player - circle shaped
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

    if(Vector2Distance(_position, q) <= entity -> _radius){
        entity->_buffed = true;
        entity->_speedMultiplier = speedBuff;
        _healthPoints -= 1;
        return true;
    }
    return false;

    //for enemy - square shaped
    // if(CheckCollisionRecs({_position.x, _position.y, _size, _size},{entity->_position.x, entity->_position.y,entity->_size,entity->_size}))
    // {
    //     entity->_buffed = true;
    //     entity->_speedMultiplier = speedBuff;
    //     _healthPoints -= 1;
    //     return;
    // }
}


Item::Item(Vector2 pos, float sz, float spdBuff, int hp){
    // entity based variables 
    _position = pos;
    _size = sz;
    _healthPoints = hp;

    // item based variables
    speedBuff = spdBuff;
    color = BLUE;
    timer = 0;
    
    SetState(&idle);
    
}

void ItemIdle::Enter(Item& item){
    item.color = BLUE;
}

void ItemIdle::Update(Item& item, float delta_time){}

