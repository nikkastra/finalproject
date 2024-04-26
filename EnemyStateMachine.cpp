#include <raylib.h>
#include <raymath.h>
#include <string>
#include <iostream>

#include "enemy.hpp"
#include "entity.hpp"

class Entity;

void Enemy::Update(float delta_time){
    // current_state->Update(*this, delta_time);
    // float cosTheta = Vector2DotProduct({1,0}, Vector2Normalize(velocity));
    // rotation = acos(cosTheta)*(180/3.14);
    // if(velocity.y < 0){
    //     rotation *= -1;
    // }

    // for the speed buff
    if(_buffed){
        _buffTimer += delta_time;
        if(_buffTimer  > 5.0f){
            _buffed = false;
            _buffTimer = 0.0f;
            _speedMultiplier = 1.0f;
        }
    }

}

void Enemy::Draw(){
    Rectangle e = {_position.x, _position.y, _size, _size};
    DrawRectanglePro(e, {_size/2, _size/2}, rotation, VIOLET);
    // DrawCircleLines(bodyCenter.x, bodyCenter.y, aggroRange, YELLOW);
    // DrawCircleLines(bodyCenter.x, bodyCenter.y, detectionRange, ORANGE); 
    // DrawCircleLines(bodyCenter.x, bodyCenter.y, attackRange, RED);
    DrawLineV(_position, Vector2Add(_position, Vector2Scale(Vector2Normalize(velocity), 50.0f)), BLACK);
}

// void Enemy::SetState(EnemyState* new_state){
//     current_state = new_state;
//     current_state->Enter(*this);
// }

// bool Enemy::HandleCollision(Entity* entity){
//     targetPos = entity->_position;
//     if(current_state == &readyingAttack || current_state == &attacking){
//         return;
//     }
//     if(attackRange + entity->_radius > Vector2Distance(bodyCenter, entity->_position)){
//         SetState(&readyingAttack);
//     } else if(detectionRange + entity->_radius > Vector2Distance(bodyCenter, entity->_position)){
//         SetState(&chasing);
//     } else if((detectionRange + entity->_radius < Vector2Distance(bodyCenter, entity->_position) && (aggroRange + entity->_radius > Vector2Distance(bodyCenter, entity->_position)))){
//         if(current_state == &chasing) SetState(&chasing);
//     } else if (aggroRange + entity->_radius < Vector2Distance(bodyCenter, entity->_position)) {
//         if(current_state != &wandering){
//             SetState(&wandering);
//         }
//     }
// }

// void Enemy::HandleEnemyCollision(Enemy* enemy){
//     if((attackRange + enemy->attackRange > Vector2Distance(bodyCenter, enemy->bodyCenter)) && (current_state != &readyingAttack || current_state != &attacking)){
//         velocity = Vector2Scale(Vector2Normalize(Vector2Subtract(bodyCenter, enemy->bodyCenter)), _speed);
//     }
// }

Enemy::Enemy(Vector2 pos, float sz, float spd, int hp){ 
    // Entity Based Variables
    _position = pos;
    _size = sz;
    _speed = spd;
    _healthPoints = hp;
    _buffTimer = 0;
    _speedMultiplier = 1.0f;
    _buffed = false;
    _damaged = false;
    _damage = 2;

    // Enemy Based Variables
    timer = 0;
    bodyCenter = _position;
    aggroRange = 3*sz + 50;
    detectionRange = 2*sz + 50;
    attackRange = sz + 50;


    // SetState(&wandering);
    
}

// void EnemyWandering::Enter(Enemy& enemy){
//     enemy._color = PINK;
//     enemy.velocity = {(enemy._speed * enemy._speedMultiplier)*GetRandomValue(-100, 100)/100, (enemy._speed * enemy._speedMultiplier)*GetRandomValue(-100, 100)/100};
// }

// void EnemyWandering::Update(Enemy& enemy, float delta_time){
//     enemy._position = Vector2Add(enemy._position, Vector2Scale(enemy.velocity, delta_time));
//     enemy.bodyCenter = Vector2Add(enemy.bodyCenter, Vector2Scale(enemy.velocity, delta_time));
//     enemy.timer += delta_time;
//     if(enemy.timer > 1){
//         enemy.timer = 0;
//         enemy.velocity = {(enemy._speed * enemy._speedMultiplier)*GetRandomValue(-100, 100)/100, (enemy._speed * enemy._speedMultiplier)*GetRandomValue(-100, 100)/100};
//     }
// }

// void EnemyChasing::Enter(Enemy& enemy){
//     enemy._color = RED;
//     enemy.velocity = Vector2Scale(Vector2Normalize(Vector2Subtract(enemy.targetPos, enemy.bodyCenter)), (enemy._speed * enemy._speedMultiplier));
// }

// void EnemyChasing::Update(Enemy& enemy, float delta_time){
//     enemy._position = Vector2Add(enemy._position, Vector2Scale(enemy.velocity, delta_time));
//     enemy.bodyCenter = Vector2Add(enemy.bodyCenter, Vector2Scale(enemy.velocity, delta_time));
// }

// void EnemyReadyingAttack::Enter(Enemy& enemy){
//     enemy._color = MAROON;
// }

// void EnemyReadyingAttack::Update(Enemy& enemy, float delta_time){
//     enemy.velocity = Vector2Scale(Vector2Normalize(Vector2Subtract(enemy.targetPos, enemy.bodyCenter)), (enemy._speed * enemy._speedMultiplier));
//     enemy.timer += delta_time;
//     if(enemy.timer > 1){
//         enemy.timer = 0;
//         enemy.SetState(&enemy.attacking);
//     }
// }

// void EnemyAttacking::Enter(Enemy& enemy){
//     enemy._color = MAGENTA;
// }

// void EnemyAttacking::Update(Enemy& enemy, float delta_time){
//     enemy._position = Vector2Add(enemy._position, Vector2Scale(Vector2Scale(enemy.velocity, delta_time), 10.0f));
//     enemy.bodyCenter = Vector2Add(enemy.bodyCenter, Vector2Scale(Vector2Scale(enemy.velocity, delta_time), 10.0f));
//     enemy.timer += delta_time;
//     if(enemy.timer > 0.5){
//         enemy.timer = 0;
//         enemy.SetState(&enemy.wandering);
//     }
// }