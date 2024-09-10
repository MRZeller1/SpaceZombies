#ifndef BULLET_H
#define BULLET_H

#include "forward_declarations.h"
#include "raylib.h"

class Bullet
{
private:
    // Bullet properties
    int size;
    float speed;
    float damage;
    float lifetime;
    float remainingLifetime;
    int id;
    bool active;
    Rectangle *collisionBox;
    Vector2 position;
    Vector2 direction;
    Grid &grid;
    CollisionMap &collisionMap;

public:
    Bullet(int size, float speed, float damage, float lifetime, Grid &grid, CollisionMap &collisionMap);
    ~Bullet();
    void setActive(bool active, Vector2 position, Vector2 direction);
    void update(float deltaTime);
    void draw();
    void deactivate();
    float getDamage() { return damage; };
    Rectangle getCollisionBox() { return *collisionBox; };
    void setCollisionBox(Rectangle *collisionBox) { this->collisionBox = collisionBox; };
    bool isActive() { return active; };
};

#endif 