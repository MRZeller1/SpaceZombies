#ifndef BUG_H
#define BUG_H

static int bugCount = 0;

#include "forward_declarations.h"
#include "raylib.h"
#include "npc.h"


float const BUG_DEFAULT_SPEED = 75.0f;
float const BUG_DAMAGE = 5.0f;
const int BUG_HEALTH = 50;
class Bug : public Npc
{
private:
public:
    Bug(Grid &grid, CollisionMap &collisionMap);
    ~Bug();
    void loadTextures();
    void spawnBug(float startx, float starty);
    static void incrementBugCount();
    static int getBugCount();
};

#endif
