#ifndef PLAYER_H
#define PLAYER_H
#include <forward_declarations.h>
#include <raylib.h>
#include "character.h"

const float DEFAULT_PLAYER_SPEED = 150.0f;
const int DEFAULT_PLAYER_HEALTH = 100;
const int ANIMATION_PLAYER_SPEED = 15;
const float sprintDuration = 2.5f;
static int collisionID_players = 0;



class Player : public Character
{
private:
    // individual player attributes

    // Player possition and camera

    // Player movement/animation variables

    bool isSprinting = false;
    float sptrintcooldownTimer = 0.0;
    float sprintTimer = 0.0f;
    const float sprintCooldown = 30.0f;
    Pistols *pistols;
    Weapon *currentWeapon;
    /// FlameThrower *flameThrower;

    Camera2D camera;

public:
    Player(Pistols pistols, Grid &grid, CollisionMap &collisionMap);
    ~Player();
    // Input for movement

    void update(float deltaTime, const std::vector<GameObject *> objects) override;
    void handleMovementInput(float deltaTime);
    void handleSprinting(float deltaTime);
    void spawnPlayer(float startx, float starty);
    void respawnPlayer();
    void changeWeapon();
    void fireWeapon();
    void reloadWeapon();
    
    Weapon *getWeapon() const
    {
        return currentWeapon;
    }
    // Visual instanciation of player
    // getters for positions
    // Functions to get and update camera positon
    Camera2D getCamera() const
    {
        return camera;
    }

    void updateCamera();

    void loadTextures();
};
#endif