#ifndef NPC_H
#define NPC_H
#include <raylib.h>
#include "character.h"

const float NPC_DEFAULT_SPEED = 75.0f;
const int NPC_DEFAULT_HEALTH = 100;
const int NPC_ANIMATION_SPEED = 15;

class Npc : public Character
{
private:
    Vector2 velocity;
    float maxSpeed;
    float acceleration;
    float deceleration;
    Vector2 targetDirection;
    float turningSpeed;
public:
    // Zombie constructor and destructor
    Npc(Grid &grid, CollisionMap &collisionMap);

    ~Npc();
    // Zombie update and draw functions
    void update(float deltaTime, const std::vector<GameObject *> objects) override;
    // Zombie pathfinding vector field
    void setCellAttributes(int startx, int starty);
    void setNewDirection(Vector2 newDirection);
    void setMovement(float deltaTime);
    void loadTextures();
    bool isDead() const { return !alive; };
    float moveTowards(float current, float target, float maxChange);
};

#endif