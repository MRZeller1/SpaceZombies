#include "weapon.h"
#include "raylib.h"

Weapon::Weapon(int damage, int ammo, int maxAmmo, float fireRate, float reloadTime, bool isFlameThrower, bool isRifle, CollisionMap &collisionMap) : collisionMap(collisionMap)
{
    this->damage = damage;
    this->ammo = ammo;
    this->maxAmmo = maxAmmo;
    this->fireRate = fireRate;
    this->reloadTime = reloadTime;
    this->timeSinceLastShot = 0;
    this->timeSinceReload = 0;
    this->reloading = false;
    this->outOfAmmo = false;
    this->isFlameThrower = isFlameThrower;
    this->isRifle = isRifle;
}
Weapon::~Weapon() {
    // Implement any necessary cleanup here
    // For example, if you're managing any dynamically allocated memory
}
void Weapon::update(float deltaTime)
{
    timeSinceLastShot += deltaTime;
    if (reloading)
    {
        timeSinceReload += deltaTime;
        if (timeSinceReload >= reloadTime)
        {
            reloading = false;
            outOfAmmo = false;
        }
    }
    if(isFiring == true)
    {
        timeSinceLastShot += deltaTime;
        if(timeSinceLastShot >= fireRate)
        {
            isFiring = false;
        }
    }
}
void Weapon::fire(Vector2 position, Vector2 direction)
{
    if (timeSinceLastShot >= fireRate)
    {   
        isFiring = true;
        if (bulletsInMagazine > 0)
        {   
            createBullet(position, direction);
            bulletsInMagazine--;
            timeSinceLastShot = 0;
        }
        else
        {
            outOfAmmo = true;
            reload();
        }
    }
}
void Weapon::reload()
{       // reload timer

        reloading = true;
        timeSinceReload = 0;
        ammo -= magazineSize - bulletsInMagazine;
        bulletsInMagazine = magazineSize;
    
}

void Weapon::createBullet(Vector2 position, Vector2 direction) {
    // Implement bullet creation logic here
    // For example:
    // Bullet* newBullet = new Bullet(position, direction, bulletSpeed, bulletDamage, bulletLifeSpan);
    // bullets.push_back(newBullet);
}
