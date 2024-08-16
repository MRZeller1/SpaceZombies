#ifndef WEAPON_H
#define WEAPON_H
#include <raylib.h>
#include <vector>
#include "bullet.h"



class Weapon
{
protected:
    int damage;

    // Weapon class with two subclasses: FlameThrower and Rifle
    // FlameThrower and Rifle have different damage values
    

    int ammo;
    int maxAmmo;
    float fireRate;
    float reloadTime;
    float timeSinceLastShot;
    bool isPistols;
    bool isFiring;
    float timeSinceReload;
    bool reloading;
    bool outOfAmmo;
    bool isFlameThrower;
    bool isRifle;
    bool activeWeapon;
    int magazineSize;
    int bulletsInMagazine;

    int bulletSize;
    int bulletSpeed;
    int bulletDamage;
    int bulletLifeSpan;
    std::vector<Bullet> bullets;
    CollisionMap &collisionMap;



public:

    Weapon(int damage, int ammo, int maxAmmo, float fireRate, float reloadTime, bool isFlameThrower, bool isRifle, CollisionMap &collisionMap);
    ~Weapon();
    void update(float deltaTime);
    void reload();
    void canShoot();
    bool canReload();
    virtual void fire(Vector2 position, Vector2 direction);
    void createBullet(Vector2 position, Vector2 direction);
    bool getIsFlameThrower(){return isFlameThrower;};
    bool getIsRifle(){return isRifle;};
    bool getReloading(){return reloading;};
    void setReloading(bool reloading){this->reloading = reloading;};
    void setActiveWeapon(bool activeWeapon){this->activeWeapon = activeWeapon;};



};
#endif