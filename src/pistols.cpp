#include "pistols.h"

Pistols::Pistols(CollisionMap &collisonMap) : Weapon(10, 100, 100, 0.5f, 1.0f, false, false, collisonMap)
{
    bulletSize = 5;
    bulletSpeed = 500;
    bulletDamage = 10;
    bulletLifeSpan = 1;
    //this->collisionMap = collisionMap;
}
void Pistols::fire(Vector2 position, Vector2 direction) {
    float currentTime = GetTime();
    if (currentTime - lastFireTime < fireRate || ammo <= 1) return;

    lastFireTime = currentTime;
    ammo -= 2;

    // Fire two bullets
    for (int i = 0; i < 2; i++) {
        createBullet(position, direction);
    }
}