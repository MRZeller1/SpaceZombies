#ifndef NPC_H
#define NPC_H
#include <raylib.h>
#include "character.h"

const float NPC_DEFAULT_SPEED = 75.0f;
const int NPC_DEFAULT_HEALTH = 100;
const int NPC_ANIMATION_SPEED = 15;
static int collisionID_npcs = 0;

class Npc : public Character
{
private:
    Vector2 velocity;
    float maxSpeed;
    float acceleration;
    float deceleration;
    Vector2 targetDirection;
    float turningSpeed;
    float damage;

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
    void incrementCollisionID() { collisionID_npcs++; };
    void decrementCollisionID() { collisionID_npcs--; };
    int getCollisionID() { return collisionID_npcs; };
    float moveTowards(float current, float target, float maxChange);
    float getDamage() {return damage; };
    void setDamage(float damage) {this->damage = damage; };
};

#endif