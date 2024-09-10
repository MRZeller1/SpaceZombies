#include "bug.h"
#include "grid.h"
#include "collisionMap.h"

Bug::Bug(Grid &grid, CollisionMap &collisionMap) : Npc(grid, collisionMap)
{
    loadTextures();
    currentTexture = leftTexture;
    alive = false;
    setDamage(BUG_DAMAGE);
    incrementBugCount();
}

void Bug::loadTextures()
{
    leftTexture = LoadTexture("resources/bug.png");
    leftWalkTexture1 = LoadTexture("resources/bug.png");
    leftWalkTexture2 = LoadTexture("resources/bug.png");
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
}

void Bug::incrementBugCount()
{
    bugCount++;
}

int Bug::getBugCount()
{
    return bugCount;
}