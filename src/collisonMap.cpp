#include "collisionMap.h"


CollisionMap::CollisionMap(int boundsWidth, int boundsHeight) {
    {
        std::cout << "Map created!" << "\n";
        collisionMap = std::unordered_map<int, Rectangle*>();
        staticCollisionRectangles = std::vector<Rectangle*>();
        mapRanges = std::vector<int>();
        for(int i = 0; i < 3; i++){
           mapRanges.push_back(0);
        }
        this->boundsWidth = boundsWidth;
        this->boundsHeight = boundsHeight;

    }
    
}
void CollisionMap::setBounds(int width, int height) {
    boundsWidth = width;
    boundsHeight = height;
}
void CollisionMap::addStaticCollisionRectangle(Rectangle* rec) {
    staticCollisionRectangles.push_back(rec);
}
void CollisionMap::addDynamicCollisionRectangle(int id, Rectangle* rec) {
    collisionMap[id] = rec;
}
bool CollisionMap::checkCollision(const Rectangle& rec1, const Rectangle& rec2) {
    return CheckCollisionRecs(rec1, rec2);
}
bool CollisionMap::checkCollision(const Rectangle& rec1, int ignoreID) {
        for(int i = 1001; i<= mapRanges[0]; i++){
            if (CheckCollisionRecs(rec1, *collisionMap[i]) && i != ignoreID) {
                return true;
            }
        }
    
        for(int i = 2001; i <= mapRanges[1]; i++){
            if (CheckCollisionRecs(rec1, *collisionMap[i]) && i != ignoreID) {
                return true;
            }
        }
        for(int i = 3001; i <= mapRanges[2]; i++){
            if (CheckCollisionRecs(rec1, *collisionMap[i]) && i != ignoreID) {
                return true;
            }
        }
        for (Rectangle* rec : staticCollisionRectangles) {
            if (CheckCollisionRecs(rec1, *rec)) {
                return true;
            }
        }
    
    return false;
}
void CollisionMap::updateDynamicCollisionRectangle(int id, Rectangle* rec) {
    collisionMap[id] = rec;
}
bool CollisionMap::checkBounds(const Rectangle& rec1) {
    
    if (rec1.x <= 0 || rec1.x >= boundsWidth || rec1.y <= 0 || rec1.y >= boundsHeight)
    {
        return true;
    }
    return false;
    
}
void CollisionMap::updateMapRangesPlayer(int range) {
    mapRanges[0] = range;
}
void CollisionMap::updateMapRangesZombie(int range) {
    mapRanges[1] = range;
}
void CollisionMap::updateMapRangesBug(int range) {
    mapRanges[2] = range;
}