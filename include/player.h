#ifndef PLAYER_H
#define PLAYER_H
#include <forward_declarations.h>
#include <raylib.h>
#include "character.h"

const float DEFAULT_PLAYER_SPEED = 150.0f;
const int DEFAULT_PLAYER_HEALTH = 100;
const int ANIMATION_PLAYER_SPEED = 15;
const int UPDATE_RADIUS = 50;
const float sprintDuration = 2.5f;
static int collisionID_players = 0;



class Player : public Character
{
private:

    bool isSprinting = false;
    float sptrintcooldownTimer = 0.0;
    float sprintTimer = 0.0f;
    const float sprintCooldown = 30.0f;
    Pistols *pistols;
    Weapon *currentWeapon;

    Camera2D camera;

public:
    Player(Pistols* pistols, Grid &grid, CollisionMap &collisionMap);
    ~Player();
    void update(float deltaTime, const std::vector<GameObject *> objects) override;
    void handleMovementInput(float deltaTime);
    void handleSprinting(float deltaTime);
    void spawnPlayer(float startx, float starty);
    void changeWeapon();
    void fireWeapon();
    void reloadWeapon();
    void updateGridPosition();
    int getHealth() const { return health; }
    int getX(){return gridPos.x;}
    int getY(){return gridPos.y;}
    Vector2 getPosition(){return position;}
    Weapon *getWeapon() const{ return currentWeapon; }
    Camera2D getCamera() const {return camera;}
    void updateCamera();
    void loadTextures();
    void setHealth(int health) { this->health = health; }
    int getTextureHeight(){return currentTexture.height;}
    int getTextureWidth(){return currentTexture.width;}
};
#endif