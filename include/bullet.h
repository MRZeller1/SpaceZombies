#ifndef BULLET_H
#define BULLET_H

#include "forward_declarations.h"
#include "raylib.h"

static int collisionID_bullets = 0;

class Bullet
{
private:
    int size;
    float speed;
    float damage;
    float x;
    float y;
    int id;
    Vector2 direction;
    float lifetime;
    bool active;
    float remainingLifetime;
    Rectangle *collisionBox;
    Vector2 position;
    Grid &grid;
    CollisionMap &collisionMap;

public:
    Bullet(int size, float speed, float damage, float lifetime, Grid &grid, CollisionMap &collisionMap);
    ~Bullet();
    void setActive(bool active, Vector2 position, Vector2 direction);
    void update(float deltaTime);
    void draw();
    void checkCollision();
    float getDamage(){ return damage; };
    Rectangle getCollisionBox(){ return *collisionBox;};
    void setCollisionBox(Rectangle *collisionBox){ this->collisionBox = collisionBox;};
    bool isActive() { return active; };
    void deactivate();
};
#endif // BULLET_H