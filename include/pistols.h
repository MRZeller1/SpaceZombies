#ifndef PISTOLS
#define PISTOLS

#include "weapon.h"
#include "raylib.h"
#include <vector>
#include <iostream>
int BULLET_SIZE = 5;
float BULLET_SPEED = 500;
float BULLET_DAMAGE = 10;
float BULLET_LIFESPAN = 1;

class Pistols : public Weapon
{
private:
std::vector<Bullet> bullets;
float lastFireTime = 0;

public:
    Pistols(CollisionMap &collisionMap) : Weapon(10, 100, 100, 0.5f, 1.0f, false, false, collisionMap)
    {
        bulletSize = BULLET_SIZE;
        bulletSpeed = BULLET_SPEED;
        bulletDamage = BULLET_DAMAGE;
        bulletLifeSpan = BULLET_LIFESPAN;
    }
    ~Pistols() {}
    void fire(Vector2 postision , Vector2 direction);
    
};

#endif