#include "bug.h"
#include "textures.h"
#include "grid.h"
#include "collisionMap.h"

Bug::Bug(Grid &grid, CollisionMap &collisionMap) : Npc(grid, collisionMap)
{
    loadTextures();
    currentTexture = leftTexture;
    alive = false;
    health = BUG_HEALTH;
    setDamage(BUG_DAMAGE);
    incrementBugCount();
}

void Bug::loadTextures()
{
    leftTexture = GameTextures::Bug();
    leftWalkTexture1 = leftTexture;
    leftWalkTexture2 = leftTexture;
}

void Bug::spawnBug(float startx, float starty)
{

    setPos(startx, starty);
    Vector2 gridPos = grid.getGridPosition(startx, starty);
    setCellAttributes(startx, starty);
    collisionBox = new Rectangle{position.x - currentTexture.width / 2, position.y - currentTexture.height / 2, (float)currentTexture.width, (float)currentTexture.height};
    incrementCollisionID();
    personalCollisionID = getCollisionID();
    collisionMap.addNPCCollisionRectangle(personalCollisionID, this);

    alive = true;
    health = BUG_HEALTH;
}

void Bug::incrementBugCount()
{
    bugCount++;
}

int Bug::getBugCount()
{
    return bugCount;
}