#include "pistols.h"
#include "bullet.h"

Pistols::Pistols(std::vector<Bullet *> pistolBullets) : Weapon(10, 100, 100, 0.5f, 1.0f, false, false, pistolBullets)
{
    bulletSize = 5;
    bulletSpeed = 500;
    bulletDamage = 10;
    bulletLifeSpan = 1;
    // this->collisionMap = collisionMap;
}
void Pistols::fire(Vector2 position, Vector2 direction) {
    float currentTime = GetTime();
    if (currentTime - lastFireTime < fireRate || ammo <= 1)
        return;

    lastFireTime = currentTime;
    ammo -= 2;

    // Fire two bullets
    for (int i = 0; i < 2; i++) {
        Bullet* bullet = getAvailableBullet();
        if (bullet) {
            Vector2 offsetPosition = {
                position.x + direction.x * 20,  // Offset bullet spawn point
                position.y + direction.y * 20
            };
            bullet->setActive(true, offsetPosition, direction);
        }
    }
}

Bullet* Pistols::getAvailableBullet() {
    for (Bullet* bullet : bullets) {
        if (!bullet->isActive()) {
            return bullet;
        }
    }
    return nullptr;  // No available bullets
}
