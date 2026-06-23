#ifndef WEAPON_H
#define WEAPON_H

#include "forward_declarations.h"
#include <raylib.h>
#include <vector>

enum class WeaponType
{
    Pistol,
    Rifle,
    Flamethrower
};

class Weapon
{
private:
    WeaponType type;
    int ammo;
    int maxAmmo;
    float fireRate;
    float reloadTime;
    float lastFireTime;
    float reloadTimer;
    float muzzleFlashTimer;
    bool reloading;
    int bulletsPerShot;
    int ammoCost;
    bool automatic;
    float spawnOffset;
    int bulletSize;
    float bulletSpeed;
    float bulletDamage;
    float bulletLifetime;
    std::vector<Bullet *> &bullets;

    Bullet *getAvailableBullet();
    void applyConfig(WeaponType weaponType);

public:
    Weapon(WeaponType weaponType, std::vector<Bullet *> &bulletPool);

    void update(float deltaTime);
    void fire(Vector2 position, Vector2 direction);
    void reload();
    void draw(Vector2 position, Vector2 direction);

    bool isAutomatic() const { return automatic; }
    bool isReloading() const { return reloading; }
    WeaponType getType() const { return type; }
    int getAmmo() const { return ammo; }
    int getMaxAmmo() const { return maxAmmo; }
    const char *getName() const;
};

#endif
