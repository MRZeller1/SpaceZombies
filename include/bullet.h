#ifndef BULLET_H
#define BULLET_H
#include "raylib.h"
#include "collisionMap.h"
#include "grid.h"

class Bullet
{
private:
    int size;
    float speed;
    float damage;
    float x;
    float y;
    float direction;
    float lifetime;
    bool active;
    Grid &grid;
    CollisionMap &collisionMap;


public:
    Bullet(int size, float speed, float damage, float lifetime, Grid &grid , CollisionMap &collisionMap);
    ~Bullet();
    void setActive(bool active, Vector2 position, Vector2 direction);
    void deactivate();

};
#endif // BULLET_H