#include "weapon.h"
#include "bullet.h"
#include <cmath>

namespace
{
void drawOrientedBox(Vector2 center, float length, float width, Vector2 dir, Color color)
{
    float px = -dir.y;
    float py = dir.x;
    float halfLen = length * 0.5f;
    float halfWid = width * 0.5f;

    Vector2 v1 = {center.x + dir.x * halfLen + px * halfWid, center.y + dir.y * halfLen + py * halfWid};
    Vector2 v2 = {center.x + dir.x * halfLen - px * halfWid, center.y + dir.y * halfLen - py * halfWid};
    Vector2 v3 = {center.x - dir.x * halfLen - px * halfWid, center.y - dir.y * halfLen - py * halfWid};
    Vector2 v4 = {center.x - dir.x * halfLen + px * halfWid, center.y - dir.y * halfLen + py * halfWid};

    DrawTriangle(v1, v2, v3, color);
    DrawTriangle(v1, v3, v4, color);
}
}

Weapon::Weapon(WeaponType weaponType, std::vector<Bullet *> &bulletPool)
    : type(weaponType), bullets(bulletPool), lastFireTime(0), reloadTimer(0), muzzleFlashTimer(0), reloading(false)
{
    applyConfig(weaponType);
}

void Weapon::applyConfig(WeaponType weaponType)
{
    switch (weaponType)
    {
    case WeaponType::Pistol:
        maxAmmo = 12;
        fireRate = 0.35f;
        reloadTime = 1.2f;
        bulletsPerShot = 1;
        ammoCost = 1;
        automatic = false;
        spawnOffset = 18.0f;
        bulletSize = 3;
        bulletSpeed = 420.0f;
        bulletDamage = 50.0f;
        bulletLifetime = 1.2f;
        break;
    case WeaponType::Rifle:
        maxAmmo = 30;
        fireRate = 0.15f;
        reloadTime = 1.8f;
        bulletsPerShot = 1;
        ammoCost = 1;
        automatic = true;
        spawnOffset = 24.0f;
        bulletSize = 3;
        bulletSpeed = 550.0f;
        bulletDamage = 18.0f;
        bulletLifetime = 1.5f;
        break;
    case WeaponType::Flamethrower:
        maxAmmo = 80;
        fireRate = 0.07f;
        reloadTime = 2.5f;
        bulletsPerShot = 1;
        ammoCost = 1;
        automatic = true;
        spawnOffset = 16.0f;
        bulletSize = 5;
        bulletSpeed = 200.0f;
        bulletDamage = 8.0f;
        bulletLifetime = 0.35f;
        break;
    }
    ammo = maxAmmo;
}

const char *Weapon::getName() const
{
    switch (type)
    {
    case WeaponType::Pistol: return "Pistol";
    case WeaponType::Rifle: return "Rifle";
    case WeaponType::Flamethrower: return "Flamethrower";
    }
    return "Weapon";
}

void Weapon::update(float deltaTime)
{
    if (muzzleFlashTimer > 0.0f)
        muzzleFlashTimer -= deltaTime;

    if (!reloading)
        return;

    reloadTimer += deltaTime;
    if (reloadTimer >= reloadTime)
    {
        ammo = maxAmmo;
        reloading = false;
        reloadTimer = 0;
    }
}

void Weapon::fire(Vector2 position, Vector2 direction)
{
    if (reloading || ammo < ammoCost)
        return;

    float currentTime = GetTime();
    if (currentTime - lastFireTime < fireRate)
        return;

    lastFireTime = currentTime;
    muzzleFlashTimer = 0.08f;
    ammo -= ammoCost;

    float len = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (len < 0.01f)
        direction = {1.0f, 0.0f};
    else
    {
        direction.x /= len;
        direction.y /= len;
    }

    for (int i = 0; i < bulletsPerShot; i++)
    {
        Bullet *bullet = getAvailableBullet();
        if (!bullet)
            return;

        bullet->configure(bulletSize, bulletSpeed, bulletDamage, bulletLifetime);
        Vector2 offsetPosition = {
            position.x + direction.x * spawnOffset,
            position.y + direction.y * spawnOffset};
        bullet->setActive(true, offsetPosition, direction);
    }
}

void Weapon::reload()
{
    if (reloading || ammo >= maxAmmo)
        return;

    reloading = true;
    reloadTimer = 0;
}

void Weapon::draw(Vector2 position, Vector2 direction)
{
    float len = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (len < 0.01f)
        direction = {1.0f, 0.0f};
    else
    {
        direction.x /= len;
        direction.y /= len;
    }

    float perpX = -direction.y;
    float perpY = direction.x;

    Vector2 hand = {position.x + direction.x * 4.0f + perpX * 6.0f,
                    position.y + direction.y * 4.0f + perpY * 6.0f};

    Color slide = {58, 60, 66, 255};
    Color frame = {42, 44, 48, 255};
    Color gripColor = {32, 26, 20, 255};
    Color accent = {24, 24, 28, 255};

    if (type == WeaponType::Pistol)
    {
        Vector2 slideCenter = {hand.x + direction.x * 5.0f, hand.y + direction.y * 5.0f};
        Vector2 barrelCenter = {slideCenter.x + direction.x * 7.0f, slideCenter.y + direction.y * 7.0f};
        Vector2 gripCenter = {hand.x - direction.x * 2.0f + perpX * 3.0f,
                              hand.y - direction.y * 2.0f + perpY * 3.0f};
        Vector2 triggerGuard = {hand.x + direction.x * 1.0f + perpX * 2.0f,
                                hand.y + direction.y * 1.0f + perpY * 2.0f};

        drawOrientedBox(slideCenter, 11.0f, 4.0f, direction, slide);
        drawOrientedBox(barrelCenter, 7.0f, 2.5f, direction, frame);
        drawOrientedBox(gripCenter, 5.0f, 3.5f, direction, gripColor);
        drawOrientedBox(triggerGuard, 3.0f, 2.0f, direction, accent);

        Vector2 muzzle = {barrelCenter.x + direction.x * 5.0f, barrelCenter.y + direction.y * 5.0f};
        DrawCircleV(muzzle, 1.5f, frame);

        if (muzzleFlashTimer > 0.0f)
        {
            Vector2 flash = {muzzle.x + direction.x * 3.0f, muzzle.y + direction.y * 3.0f};
            DrawCircleV(flash, 4.0f, Color{255, 230, 100, 220});
        }
    }
    else if (type == WeaponType::Rifle)
    {
        Vector2 body = {hand.x + direction.x * 10.0f, hand.y + direction.y * 10.0f};
        drawOrientedBox(body, 24.0f, 3.5f, direction, slide);
        drawOrientedBox(hand, 6.0f, 3.0f, direction, gripColor);
    }
    else
    {
        Vector2 nozzle = {hand.x + direction.x * 12.0f, hand.y + direction.y * 12.0f};
        drawOrientedBox(nozzle, 10.0f, 5.0f, direction, Color{90, 90, 90, 255});
        if (muzzleFlashTimer > 0.0f)
            DrawCircleV(nozzle, 6.0f, Color{255, 120, 40, 180});
    }
}

Bullet *Weapon::getAvailableBullet()
{
    for (Bullet *bullet : bullets)
    {
        if (!bullet->isActive())
            return bullet;
    }
    return nullptr;
}
