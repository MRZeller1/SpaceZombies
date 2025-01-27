#include "zombie.h"
#include "collisionMap.h"
#include "grid.h"

Zombie::Zombie(Grid &grid, CollisionMap &collisionMap) : Npc(grid, collisionMap)
{
    loadTextures();
    currentTexture = leftTexture;
    alive = false;
    setDamage(ZOMBIE_DAMAGE);
    incrementZombieCount();
}

void Zombie::loadTextures()
{
    leftTexture = LoadTexture("resources/zombie.png");
    leftWalkTexture1 = LoadTexture("resources/zombie_walk1.png");
    leftWalkTexture2 = LoadTexture("resources/zombie_walk2.png");
}
void Zombie::spawnZombie(float startx, float starty)

{
    setPos(startx, starty);
    setCellAttributes(startx, starty);
    collisionBox = new Rectangle{position.x - currentTexture.width / 2, position.y - currentTexture.height / 2, (float)currentTexture.width - (float)currentTexture.width * 0.1f, (float)currentTexture.height - (float)currentTexture.height * 0.1f};
    incrementCollisionID();
    personalCollisionID = getCollisionID();
    collisionMap.addNPCCollisionRectangle(personalCollisionID, this);
    alive = true;
}

void Zombie::incrementZombieCount()
{
    zombieCount++;
}

int Zombie::getZombieCount()
{
    return zombieCount;
}