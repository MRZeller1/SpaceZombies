
#ifndef BUG_H
#define BUG_H
static int collisionID_bugs = 3000;
static int bugCount = 0;
#include "npc.h"

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


