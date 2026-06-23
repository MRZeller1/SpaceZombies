#include "bullet.h"
#include "collisionMap.h"

static int collisionID_bullets = 0;

Bullet::Bullet(int size, float speed, float damage, float lifetime, Grid &grid, CollisionMap &collisionMap)
    : size(size), speed(speed), damage(damage), lifetime(lifetime), grid(grid), collisionMap(collisionMap)
{
    id = collisionID_bullets++;
    active = false;
    collisionBox = {0, 0, 0, 0};
}

Bullet::~Bullet()
{
    if (active)
        collisionMap.removeBulletCollisionRectangle(id);
}

void Bullet::configure(int bulletSize, float bulletSpeed, float bulletDamage, float bulletLifetime)
{
    size = bulletSize;
    speed = bulletSpeed;
    damage = bulletDamage;
    lifetime = bulletLifetime;
}

void Bullet::setActive(bool active, Vector2 position, Vector2 direction)
{
    this->active = active;
    this->position = position;
    this->direction = direction;
    this->remainingLifetime = lifetime;

    if (active)
    {
        collisionBox = {
            position.x - size / 2.0f,
            position.y - size / 2.0f,
            (float)size,
            (float)size};
        collisionMap.addBulletCollisionRectangle(id, this);
    }
    else
    {
        collisionMap.removeBulletCollisionRectangle(id);
    }
}

void Bullet::update(float deltaTime)
{
    if (!active)
        return;

    position.x += direction.x * speed * deltaTime;
    position.y += direction.y * speed * deltaTime;

    collisionBox = {
        position.x - size / 2.0f,
        position.y - size / 2.0f,
        (float)size,
        (float)size};

    remainingLifetime -= deltaTime;
    if (remainingLifetime <= 0)
    {
        deactivate();
        return;
    }

    collisionMap.checkBulletHits(this);
}

void Bullet::draw()
{
    if (!active)
        return;

    DrawRectangle(position.x - size / 2, position.y - size / 2, size, size, RED);
}

void Bullet::deactivate()
{
    active = false;
    collisionMap.removeBulletCollisionRectangle(id);
}
