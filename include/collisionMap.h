#ifndef COLLISIONMAP_H
#define COLLISIONMAP_H

#include <raylib.h>
#include <vector>
#include <unordered_map>
#include "forward_declarations.h"




class CollisionMap
{
private:
    std::unordered_map<int, Player*> playerMap;
    std::unordered_map<int, Npc*> npcMap;
    std::unordered_map<int, Bullet*> bulletMap;
    std::vector<Rectangle *> staticCollisionRectangles;
    std::vector<Rectangle *> playerOnlyCollisionRectangles;
    std::vector<int> mapRanges;

    Player *currentPlayer;
    Npc *currentNpc;
    Bullet *currentBullet;

    int boundsWidth;
    int boundsHeight;

public:
    CollisionMap(int boundsWidth, int boundsHeight);
    
    //Add dynamic collision rectangles

    void addStaticCollisionRectangle(Rectangle *rec);

    void addPlayerCollisionRectangle(int id, Player *player);

    void addNPCCollisionRectangle(int id,  Npc *npc);

    void addBulletCollisionRectangle(int id, Bullet *bullet);
    
    //Remove dynamic collion rectangles

    void removePlayerCollisionRectangle(int id){ playerMap.erase(id); };

    void removeNPCCollisionRectangle(int id){ npcMap.erase(id); };

    void removeBulletCollisionRectangle(int id){ bulletMap.erase(id); };

    //Check for collision between two rectangles

    bool checkCollision(const Rectangle &rec1, const Rectangle &rec2);

    bool checkPlayerCollision(const Rectangle &rec1, int ignoreID, Character *player);

    bool checkNPCCollision(const Rectangle &rec1, int ignoreID, Character *npc);

    //Check for collision between a rectangle and a list of rectangles

    bool isPlayerCollision(const Rectangle &rec1, int ignoreID, bool npc);

    bool isNPCCollision(const Rectangle &rec1, int ignoreID);

    bool isBulletCollision(const Rectangle &rec1, int ignoreID);

    bool isStaticCollision(const Rectangle &rec1);

    void handleBulletCollision(const Rectangle &rec1, int ignoreID);

    void setBounds(int width, int height);

    bool checkBounds(const Rectangle &rec1);

    //Update the collision rectangle of a dynamic object

    void updatePlayerCollisionRectangle(int id, Rectangle *rec);

    void updateNPCCollisionRectangle(int id, Rectangle *rec);

    void updateBulletCollisionRectangle(int id, Rectangle *rec);
};
#endif