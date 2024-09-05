#include "npc.h"
#include <cmath> // Add this for fabs function
#include "grid.h"
#include "collisionMap.h"

Npc::Npc(Grid &grid, CollisionMap &collisionMap) : Character(grid, collisionMap, NPC_DEFAULT_SPEED, NPC_DEFAULT_HEALTH)
{
    type = NPC_TYPE;
    direction = {1, 0};
    previousDirection = {1, 0};
    speed = NPC_DEFAULT_SPEED;
    animation = NPC_ANIMATION_SPEED;

    // New variables for momentum-based movement
    velocity = {0, 0};
    maxSpeed = NPC_DEFAULT_SPEED;
    acceleration = 100.0f; // Adjust as needed
    deceleration = 200.0f; // Adjust as needed
    targetDirection = direction;
    turningSpeed = 2.0f; // Adjust as needed
}

Npc::~Npc()
{
    UnloadTexture(leftTexture);
    UnloadTexture(leftWalkTexture1);
    UnloadTexture(leftWalkTexture2);
}


void Npc::update(float deltaTime, const std::vector<GameObject *> objects)
{
    if (getCellDistance() != 100)
    {
        Vector2 newDirection = getCellDirection();
        if (newDirection.x != targetDirection.x || newDirection.y != targetDirection.y)
        {
            previousDirection = direction;
            targetDirection = newDirection;
        }
    }

    // Gradually turn towards the target direction
    direction.x = moveTowards(direction.x, targetDirection.x, turningSpeed * deltaTime);
    direction.y = moveTowards(direction.y, targetDirection.y, turningSpeed * deltaTime);

    setMovement(deltaTime);
    updatePosition(deltaTime, objects);
    updateGridPosition();
    updateAnimation();
}

void Npc::setCellAttributes(int startx, int starty)
{
    Vector2 gridPos = grid.getGridPosition(startx, starty);
    grid.setCellAttributes(gridPos.x, gridPos.y, NPC_TYPE);
}

void Npc::setMovement(float deltaTime)
{
    // Comment out the old movement calculation
    /*
    if (!colliding)
        isMoving = true;
    else
        isMoving = false;

    movement = {direction.x * speed * deltaTime, direction.y * speed * deltaTime};
    */

    // New momentum-based movement calculation
    Vector2 targetVelocity = {direction.x * maxSpeed, direction.y * maxSpeed};

    // Apply acceleration towards the target velocity
    velocity.x = moveTowards(velocity.x, targetVelocity.x, acceleration * deltaTime);
    velocity.y = moveTowards(velocity.y, targetVelocity.y, acceleration * deltaTime);

    // Apply deceleration if not moving in that direction
    if (fabs(direction.x) < 0.1f)
        velocity.x = moveTowards(velocity.x, 0, deceleration * deltaTime);
    if (fabs(direction.y) < 0.1f)
        velocity.y = moveTowards(velocity.y, 0, deceleration * deltaTime);

    // Set the movement based on current velocity
    movement = {velocity.x * deltaTime, velocity.y * deltaTime};

    isMoving = (fabs(velocity.x) > 0.1f || fabs(velocity.y) > 0.1f);
}

// New helper function for gradual value changes
float Npc::moveTowards(float current, float target, float maxChange)
{
    if (current < target)
    {
        return std::min(current + maxChange, target);
    }
    else if (current > target)
    {
        return std::max(current - maxChange, target);
    }
    return target;
}