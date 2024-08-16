#ifndef PISTOLS
#define PISTOLS

#include "weapon.h"
#include "raylib.h"
#include <vector>
#include <iostream>
/*int PISTOL_BULLET_SIZE = 5;
float PISTOL_BULLET_SPEED = 500;
float PISTOL_BULLET_DAMAGE = 10;
float PISTOL_BULLET_LIFESPAN = 1;
*/
class Pistols : public Weapon
{
private:
std::vector<Bullet> bullets;
float lastFireTime = 0;

public:
    Pistols(CollisionMap &collisionMap);
    ~Pistols() {}
    void fire(Vector2 postision , Vector2 direction);
    
};

#endif