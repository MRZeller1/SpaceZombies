#include "bullet.h"

Bullet::Bullet(int size, float speed, float damage, float lifetime, Grid &grid, CollisionMap &collisionMap) : size(size), speed(speed), damage(damage), lifetime(lifetime), grid(grid), collisionMap(collisionMap)
{
    active = false;
}

Bullet::~Bullet()
{
}

void Bullet::setActive(bool active, Vector2 position, Vector2 direction)
{
    this->active = active;
    x = position.x;
    y = position.y;
    this->direction = direction;
}

void Bullet::deactivate()
{
    active = false;
}