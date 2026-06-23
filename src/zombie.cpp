#include "zombie.h"
#include "textures.h"
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
    leftTexture = GameTextures::Zombie();
    leftWalkTexture1 = GameTextures::ZombieWalk1();
    leftWalkTexture2 = GameTextures::ZombieWalk2();
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