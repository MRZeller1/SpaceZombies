#ifndef COLLISIONMAP_H
#define COLLISIONMAP_H
#include <raylib.h>
#include "grid.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include "gameobject.h"

class CollisionMap
{
    private:
    std::unordered_map<int, Rectangle*> collisionMap;
    std::vector<Rectangle*> staticCollisionRectangles;
    std::vector<int> mapRanges;
    
    int boundsWidth;
    int boundsHeight;
    public:
    CollisionMap(int boundsWidth, int boundsHeight);

    void addStaticCollisionRectangle(Rectangle* rec);

    void addDynamicCollisionRectangle(int id, Rectangle* rec);
   
    void updateDynamicCollisionRectangle(int id, Rectangle* rec);

    bool checkCollision(const Rectangle& rec1, const Rectangle& rec2);

    bool checkCollision(const Rectangle& rec1, int ignoreID);

    void setBounds(int width, int height);
    
    bool checkBounds(const Rectangle& rec1);

    void updateMapRangesPlayer(int range);

    void updateMapRangesZombie(int range);

    void updateMapRangesBug(int range);

};
#endif