#include "character.h"
#include "collisionMap.h"
#include "raylib.h"
#include "grid.h"

void Character::updatePosition(float deltaTime, const std::vector<GameObject *> &objects)
{
    Vector2 newPos = {position.x + movement.x, position.y + movement.y};
    Rectangle newRectX = {newPos.x - currentTexture.width / 2, position.y - currentTexture.height / 2, (float)currentTexture.width, (float)currentTexture.height};
    Rectangle newRectY = {position.x - currentTexture.width / 2, newPos.y - currentTexture.height / 2, (float)currentTexture.width, (float)currentTexture.height};

    bool collisionX = false, collisionY = false;

    if (type == PLAYER_TYPE)
    {
        if (collisionMap.checkPlayerCollision(newRectX, personalCollisionID, this) || collisionMap.checkBounds(newRectX))
        {
            collisionX = true;
        }
        else if (collisionMap.checkPlayerCollision(newRectY, personalCollisionID, this) || collisionMap.checkBounds(newRectY))
        {
            collisionY = true;
        }
    }
    if (type == NPC_TYPE)
    {
        if (collisionMap.checkNPCCollision(newRectX, personalCollisionID, this) || collisionMap.checkBounds(newRectX))
        {
            collisionX = true;
        }
        else if (collisionMap.checkNPCCollision(newRectY, personalCollisionID, this) || collisionMap.checkBounds(newRectY))
        {
            collisionY = true;
        }
    }

    if (collisionX && collisionY)
        return;

    if (!collisionX)
        position.x += movement.x;
    if (!collisionY)
        position.y += movement.y;

    collisionBox->x = position.x - currentTexture.width / 2;
    collisionBox->y = position.y - currentTexture.height / 2;
    collisionBox->width = (float)currentTexture.width;
    collisionBox->height = (float)currentTexture.height;
}

void Character::updateAnimation()
{
    if (isMoving)
    {
        frameCounter++;
        if (frameCounter >= animation)
        {
            currentTexture = (walk_alt ? leftWalkTexture1 : leftWalkTexture2);
            walk_alt = !walk_alt;
            frameCounter = 0;
        }
    }
}

void Character::updateGridPosition()
{
    Vector2 newGridPos = grid.getGridPosition(position.x + movement.x, position.y + movement.y);

    if (newGridPos.x != lastKnownGridPos.x || newGridPos.y != lastKnownGridPos.y)
    {
        if (grid.isValidCell(lastKnownGridPos.x, lastKnownGridPos.y) && grid.getGridNode(lastKnownGridPos.x, lastKnownGridPos.y))
        {
            grid.getGridNode(lastKnownGridPos.x, lastKnownGridPos.y)->setAttributes(0);
        }

        if (grid.isValidCell(newGridPos.x, newGridPos.y) && grid.getGridNode(newGridPos.x, newGridPos.y))
        {
            grid.getGridNode(newGridPos.x, newGridPos.y)->setAttributes(type);
        }

        lastKnownGridPos = newGridPos;
    }
}

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

void Character::draw()
{
    DrawTexture(currentTexture, (int)position.x - currentTexture.width / 2, (int)position.y - currentTexture.height / 2, WHITE);
}

bool Character::collidesWith(const Rectangle &rect)
{
    return CheckCollisionRecs({position.x - currentTexture.width / 2, position.y - currentTexture.height / 2, (float)currentTexture.width, (float)currentTexture.height}, rect);
}

void Character::takeDamage(float damage)
{
    if (!alive) return;
    health -= damage;

    std::cout << (type == PLAYER_TYPE ? "Player health: " : "Health: ") << health << std::endl;

    if (health <= 0)
        alive = false;
}