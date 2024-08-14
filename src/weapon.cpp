#include "weapon.h"
#include "raylib.h"

Weapon::Weapon(int damage, int ammo, int maxAmmo, float fireRate, float reloadTime, bool isFlameThrower, bool isRifle)
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

