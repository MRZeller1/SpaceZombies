#ifndef PISTOLS
#define PISTOLS
#include "forward_declarations.h"
#include "weapon.h"
#include "raylib.h"
#include <vector>
#include <iostream>

class Pistols : public Weapon
{
private:
    std::vector<Bullet *> bullets;
    float lastFireTime = 0;

public:
    Pistols(std::vector<Bullet *> pistolBullets);
    ~Pistols() {}
    void fire(Vector2 postision, Vector2 direction);
    Bullet *getAvailableBullet();
};

#endif