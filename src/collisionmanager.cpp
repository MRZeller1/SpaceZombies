#include "collisionmanager.h"

bool CollisionManager::checkCollision(const Rectangle &rec1, const Rectangle &rec2)
{
    return CheckCollisionRecs(rec1, rec2);
}
bool CollisionManager::checkBounds(const Rectangle &rec1, int windowWidth, int windowHeight)
{
    if (rec1.x <= 0 || rec1.x >= windowWidth || rec1.y <= 0 || rec1.y >= windowHeight)
    {
        return true;
    }
    return false;
}