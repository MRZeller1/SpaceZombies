#ifndef CHARACTER_H
#define CHARACTER_H

#include "forward_declarations.h"
#include <raylib.h>
#include <cmath>
#include <iostream>
#include <vector>


class Character
{
protected:
    // Character attributes
    int health;
    float speed;
    float maxSpeed;
    bool alive = true;
    int type = 0;
    int const PLAYER_TYPE = 1;
    int const NPC_TYPE = 2;
    int personalCollisionID;
    // Direction and movement
    Vector2 movement;
    Vector2 position;
    Vector2 direction;
    Vector2 previousDirection;
    Rectangle *collisionBox;
    CollisionMap &collisionMap;

    Grid &grid;
    // Waling animation
    bool walk_alt = false;
    bool isMoving = false;
    bool stepping = false;
    bool colliding = false;
    int animation;
    // Dead
    // bool dead = true;

    int frameCounter = 0;

    // Textures
    Texture2D currentTexture;
    Texture2D leftTexture;
    Texture2D leftWalkTexture1;
    Texture2D leftWalkTexture2;
    Texture2D rgihtTexture;
    Texture2D rightWalkTexture1;
    Texture2D rightWalkTexture2;
    Vector2 lastKnownGridPos;

public:
    Character(Grid &grid, CollisionMap &collisionMap, int health = 100, float speed = 5.0f)
        : health(health), speed(speed), animation(0), position({0, 0}), direction({0, 0}), grid(grid), collisionMap(collisionMap)
    {
    }

    void draw();
    virtual void update(float deltaTime, const std::vector<GameObject *> objects) = 0;

    float getX() const { return position.x; }
    float getY() const { return position.y; }
    void updatePosition(float deltaTime, const std::vector<GameObject *> &objects);
    void takeDamage(float damage){if(!alive) return; health -= damage; if(health <= 0) alive = false; 
    if(type == PLAYER_TYPE) std::cout << "Player health: " << health << std::endl; else
    std::cout << "Health: " << health << std::endl;};
    void setPos(float x, float y) { this->position = {x, y}; };
    void setDirection(float x, float y) { this->direction = {x, y}; };
    bool isColliding(const std::vector<GameObject *> objects) { return false; };
    Rectangle getCollisionBox() { return *collisionBox; };
    void updateAnimation();
    void updateGridPosition();
    void setCollisionBox(Rectangle *collisionBox) { this->collisionBox = collisionBox; };
    Vector2 getCellDirection();
    int getCellDistance();
    bool collidesWith(const Rectangle &rect);
    bool isDead(){return !alive;};
};
#endif