#ifndef ITEM
#define ITEM

#include <raylib.h>
#include <raymath.h>

#include "entity.hpp"

class Item;

class ItemState{
public:
    virtual ~ItemState(){}
    virtual void Enter(Item& item) = 0;
    virtual void Update(Item& item, float delta_time) = 0;
};


class ItemIdle: public ItemState{
    void Enter(Item& item);
    void Update(Item& item, float delta_time);
};




class Item : public Entity{
public:
    
    Color color;
    float timer;
    float speedBuff;
    
    ItemIdle idle;
    
    Item(Vector2 pos, float sz, float spdBuff, int hp);
    

    void Update(float delta_time);
    
    void Draw();

    void SetState(ItemState* new_state);

    bool HandleCollision(Entity* entity,  float delta_time);

private:
    ItemState* current_state;
};
#endif