#ifndef ZOMBIE_H
#define ZOMBIE_H
#include "forward_declarations.h"
#include "npc.h"

const float ZOMBIE_DEFAULT_SPEED = 75.0f;
const int ZOMBIE_DEFAULT_HEALTH = 100;
const float ZOMBIE_DAMAGE = 25.0f;
const int ZOMBIE_ANIMATION_SPEED = 15;
static int zombieCount = 0;
class Zombie : public Npc
{
private:
public:
    // Zombie constructor and destructor
    Zombie(Grid &grid, CollisionMap &collisionMap);
    ~Zombie();
    void loadTextures();
    void spawnZombie(float startx, float starty);
    static void incrementZombieCount();
    static int getZombieCount();
};

#endif