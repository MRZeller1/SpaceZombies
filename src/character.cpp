#include "character.h"


void Character::updatePosition(float deltaTime, const std::vector<GameObject *> &objects)
{
    Vector2 newPos = {position.x + movement.x, position.y + movement.y};
    Rectangle newRectX = {newPos.x - currentTexture.width / 2, position.y - currentTexture.height / 2, (float)currentTexture.width, (float)currentTexture.height};
    Rectangle newRectY = {position.x - currentTexture.width / 2, newPos.y - currentTexture.height / 2, (float)currentTexture.width, (float)currentTexture.height};

    bool collisionX = false, collisionY = false;
    for (const GameObject *object : objects)
    {
        // cehckCollision doesnt prevent any collisions between objects or characters
        // check bounds prevents anyone from moving at all
        if (collisionMap.checkCollision(newRectX, personalCollisionID) || collisionMap.checkBounds(newRectX)){
            //if(type == 1)
                //std::cout << "Player collision!" << "\n";
            collisionX = true;
        }
        if (collisionMap.checkCollision(newRectY, personalCollisionID) || collisionMap.checkBounds(newRectY)){
            collisionY = true;
            //if(type == 1)
                //std::cout << "Player collision!" << "\n";
        }
        if (collisionX && collisionY)
            break;
    }
    if (collisionX || collisionY)
    {
        colliding = true;
    }
    else
    {
        colliding = false;
    }
    if (!collisionX)
        position.x += movement.x;
    if (!collisionY)
        position.y += movement.y;

    collisionBox->x = position.x - currentTexture.width / 2;
    collisionBox->y = position.y - currentTexture.height / 2;
    collisionBox->width = (float)currentTexture.width;
    collisionBox->height = (float)currentTexture.height;
    collisionMap.updateDynamicCollisionRectangle(personalCollisionID, collisionBox);
}
// update animation will be here
void Character::updateAnimation()
{
    if (isMoving)
    {
        frameCounter++;
        if (frameCounter >= animation)
        {
            if (!stepping)
            {
                currentTexture = (walk_alt ? leftWalkTexture1 : leftWalkTexture2);
                walk_alt = !walk_alt;
            }
            else
            {
                currentTexture = leftTexture;
            }
            stepping = !stepping;
            frameCounter = 0;
        }
    }
};
void Character::updateGridPosition()
{
    Vector2 newGridPos = grid.getGridPosition(position.x + movement.x, position.y + movement.y);

    // Only update if the current grid position is different from the last known position
    if (newGridPos.x != lastKnownGridPos.x || newGridPos.y != lastKnownGridPos.y)
    {
        // Clear the old position
        if (grid.isValidCell(lastKnownGridPos.x, lastKnownGridPos.y) && grid.getGridNode(lastKnownGridPos.x, lastKnownGridPos.y))
        {
            grid.getGridNode(lastKnownGridPos.x, lastKnownGridPos.y)->setAttributes(0);
        }

        // Set the new position
        if (grid.isValidCell(newGridPos.x, newGridPos.y) && grid.getGridNode(newGridPos.x, newGridPos.y))
        {
            if (type == 1)
                grid.resetDistance();
            grid.getGridNode(newGridPos.x, newGridPos.y)->setAttributes(type);
            if (type == 1)
                grid.getGridNode(newGridPos.x, newGridPos.y)->update();
        }

        // Update the last known position
        lastKnownGridPos = newGridPos;
    }
};

Vector2 Character::getCellDirection()
{
    Vector2 gridPos = grid.getGridPosition(position.x, position.y);
    return grid.getGridNode(gridPos.x, gridPos.y)->getDirection();
}
int Character::getCellDistance()
{
    Vector2 gridPos = grid.getGridPosition(position.x, position.y);
    return grid.getGridNode(gridPos.x, gridPos.y)->getDistance();
}
// draw
void Character::draw()
{
    DrawTexture(currentTexture, (int)position.x - currentTexture.width / 2, (int)position.y - currentTexture.height / 2, WHITE);
};

// collides with
bool Character::collidesWith(const Rectangle &rect)
{
    return CheckCollisionRecs({position.x - currentTexture.width / 2, position.y - currentTexture.height / 2, (float)currentTexture.width, (float)currentTexture.height}, rect);
};
