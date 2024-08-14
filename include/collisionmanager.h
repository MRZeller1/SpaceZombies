#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include <raylib.h>
#include <vector>
#include "gameobject.h"
#include <iostream>


class CollisionManager
{
private:
public:
    static bool checkCollision(const Rectangle &rec1, const Rectangle &rec2);
    static bool checkBounds(const Rectangle &rec1, int windowWidth, int windowHeight);
};
#endif