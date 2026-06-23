#include "collisionMap.h"
#include "npc.h"
#include "player.h"
#include "bullet.h"
#include <iostream>

CollisionMap::CollisionMap(int boundsWidth, int boundsHeight)
{
    {
        std::cout << "Map created!" << "\n";
        playerMap = std::unordered_map<int, Player*>();
        npcMap = std::unordered_map<int, Npc*>();
        bulletMap = std::unordered_map<int, Bullet*>();
        staticCollisionRectangles = std::vector<Rectangle *>();
        mapRanges = std::vector<int>();
        for (int i = 0; i < 3; i++)
        {
            mapRanges.push_back(0);
        }
        this->boundsWidth = boundsWidth;
        this->boundsHeight = boundsHeight;
    }
}

bool CollisionMap::checkPlayerCollision(const Rectangle &rec1, int ignoreID, Character* player)
{
    if(isNPCCollision(rec1, ignoreID))
    {
        // Find the colliding NPC and make it stop
        for (const auto& pair : npcMap) {
            if (CheckCollisionRecs(rec1, npcMap[pair.first]->getCollisionBox()) && pair.first != ignoreID) {
                player->takeDamage(npcMap[pair.first]->getDamage()); 
                return true;
            }
        }
    }

    if(isPlayerCollision(rec1, ignoreID, false))
    {
        return true;
    }

    if(isBulletCollision(rec1, ignoreID))
    {
        handleBulletCollision(rec1, ignoreID);
        return true;
    }

    for (Rectangle *rec : staticCollisionRectangles)
    {
        if (CheckCollisionRecs(rec1, *rec))
        {
            return true;
        }
    }

    return false;
}

bool CollisionMap::checkNPCCollision(const Rectangle &rec1, int ignoreID, Character* npc)
{
    if(isNPCCollision(rec1, ignoreID))
    {
        return true;
    }

    if(isPlayerCollision(rec1, ignoreID, true))
    {
        // NPC collides with player, stop the NPC
        return true;
    }

    if(isBulletCollision(rec1, ignoreID))
    {
        // NPC hit by bullet, take damage but don't stop
        handleBulletCollision(rec1, ignoreID);
        return false; // Return false so the NPC doesn't stop moving
    }

    // Check for collisions with static objects
    for (Rectangle *rec : staticCollisionRectangles)
    {
        if (CheckCollisionRecs(rec1, *rec))
        {
            return true;
        }
    }

    return false;
}
void CollisionMap::handleBulletCollision(const Rectangle &rec1, int characterIgnoreID)
{
    for (const auto &bulletPair : bulletMap)
    {
        Bullet *bullet = bulletPair.second;
        if (!bullet || !bullet->isActive())
            continue;
        if (!CheckCollisionRecs(rec1, bullet->getCollisionBox()))
            continue;

        for (const auto &npcPair : npcMap)
        {
            if (npcPair.first == characterIgnoreID)
            {
                npcPair.second->takeDamage(bullet->getDamage());
                bullet->deactivate();
                return;
            }
        }

        for (const auto &playerPair : playerMap)
        {
            if (playerPair.first == characterIgnoreID)
            {
                playerPair.second->takeDamage(bullet->getDamage());
                bullet->deactivate();
                return;
            }
        }
    }
}

void CollisionMap::checkBulletHits(Bullet *bullet)
{
    if (!bullet || !bullet->isActive())
        return;

    const Rectangle rec = bullet->getCollisionBox();

    for (const auto &npcPair : npcMap)
    {
        Npc *npc = npcPair.second;
        if (!npc || npc->isDead())
            continue;
        if (CheckCollisionRecs(rec, npc->getCollisionBox()))
        {
            npc->takeDamage(bullet->getDamage());
            bullet->deactivate();
            return;
        }
    }

    for (Rectangle *staticRec : staticCollisionRectangles)
    {
        if (CheckCollisionRecs(rec, *staticRec))
        {
            bullet->deactivate();
            return;
        }
    }

    if (checkBounds(rec))
        bullet->deactivate();
}

void CollisionMap::setBounds(int width, int height)
{
    boundsWidth = width;
    boundsHeight = height;
}
void CollisionMap::addStaticCollisionRectangle(Rectangle *rec)
{
    staticCollisionRectangles.push_back(rec);
}
void CollisionMap::addPlayerCollisionRectangle(int id, Player *player)
{
    
    playerMap[id] = player;
}
void CollisionMap::addNPCCollisionRectangle(int id, Npc *npc)
{
    
    npcMap[id] = npc;
}
void CollisionMap::addBulletCollisionRectangle(int id, Bullet *bullet)
{
    
    bulletMap[id] = bullet;
}

bool CollisionMap::checkCollision(const Rectangle &rec1, const Rectangle &rec2)
{
    return CheckCollisionRecs(rec1, rec2);
}

bool CollisionMap::isStaticCollision(const Rectangle &rec1)
{
    for (Rectangle *rec : staticCollisionRectangles)
    {
        if (CheckCollisionRecs(rec1, *rec))
        {
            return true;
        }
    }
    return false;
}

void CollisionMap::updatePlayerCollisionRectangle(int id, Rectangle *rec)
{
    playerMap[id]->setCollisionBox(rec);
}

void CollisionMap::updateNPCCollisionRectangle(int id, Rectangle *rec)
{
    npcMap[id]->setCollisionBox(rec);
}

void CollisionMap::updateBulletCollisionRectangle(int id, Rectangle *rec)
{
    if (rec && bulletMap.count(id))
        bulletMap[id]->setCollisionBox(*rec);
}

bool CollisionMap::checkBounds(const Rectangle &rec1)
{
    return rec1.x < 0 || rec1.y < 0 ||
           rec1.x + rec1.width > boundsWidth ||
           rec1.y + rec1.height > boundsHeight;
}

void CollisionMap::clampToBounds(float width, float height, float &x, float &y)
{
    float halfW = width / 2.0f;
    float halfH = height / 2.0f;
    float minX = halfW;
    float minY = halfH;
    float maxX = boundsWidth - halfW;
    float maxY = boundsHeight - halfH;

    if (x < minX) x = minX;
    if (x > maxX) x = maxX;
    if (y < minY) y = minY;
    if (y > maxY) y = maxY;
}


bool CollisionMap::isPlayerCollision(const Rectangle &rec1, int ignoreID, bool npc)
{
    for (auto pair : playerMap)
    {
        if (CheckCollisionRecs(rec1, playerMap[pair.first]->getCollisionBox()) && pair.first != ignoreID)
        {
            if(npc)
            {
                playerMap[pair.first]->takeDamage(npcMap[ignoreID]->getDamage());
            }
            return true;
        }
    }
    return false;
}

bool CollisionMap::isNPCCollision(const Rectangle &rec1, int ignoreID)
{
    for (auto pair : npcMap)
    {
        if (CheckCollisionRecs(rec1, npcMap[pair.first]->getCollisionBox()) && pair.first != ignoreID)
        {
            return true;
        }
    }
    return false;
}
bool CollisionMap::isBulletCollision(const Rectangle &rec1, int ignoreID)
{
    for (auto pair : bulletMap)
    {
        if (CheckCollisionRecs(rec1, bulletMap[pair.first]->getCollisionBox()) && pair.first != ignoreID)
        {
            return true;
        }
    }
    return false;
}

