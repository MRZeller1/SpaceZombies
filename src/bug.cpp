#include "bug.h"

Bug::Bug(Grid &grid, CollisionMap &collisionMap) : Npc(grid, collisionMap)
{
    loadTextures();
    currentTexture = leftTexture;
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
    grid.setCellAttributes(gridPos.x, gridPos.y,  NPC_TYPE);
    collisionBox = new Rectangle{position.x - currentTexture.width / 2, position.y - currentTexture.height / 2, (float)currentTexture.width, (float)currentTexture.height};
    collisionID_bugs++;
    personalCollisionID = collisionID_bugs;
    collisionMap.addDynamicCollisionRectangle(personalCollisionID, collisionBox);
    collisionMap.updateMapRangesBug(collisionID_bugs);
    
    
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