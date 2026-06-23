#include "player.h"
#include "collisionMap.h"
#include "grid.h"

Player::Player(std::vector<Weapon *> &weapons, Grid &grid, CollisionMap &collisionMap)
    : Character(grid, collisionMap, DEFAULT_PLAYER_HEALTH, DEFAULT_PLAYER_SPEED),
      weapons(weapons)
{
    loadTextures();
    type = 1;
    currentTexture = leftTexture;
    currentWeapon = weapons.empty() ? nullptr : weapons[0];
    spawnPlayer(320, 1000);
}

Player::~Player()
{
    UnloadTexture(leftTexture);
    UnloadTexture(leftWalkTexture1);
    UnloadTexture(leftWalkTexture2);
}

void Player::update(float deltaTime, const std::vector<GameObject *> objects)
{
    if (!alive)
        return;
    handleMovementInput(deltaTime);
    updatePosition(deltaTime, objects);
    updateGridPosition();
    handleSprinting(deltaTime);
    updateAnimation();
    if (currentWeapon)
    {
        currentWeapon->update(deltaTime);
        reloadWeapon();
        fireWeapon();
    }
    changeWeapon();
    updateCamera();
}

void Player::handleMovementInput(float deltaTime)
{
    movement = {0, 0};
    isMoving = false;
    speed = isSprinting ? DEFAULT_PLAYER_SPEED * 2.0f : DEFAULT_PLAYER_SPEED;
    animation = isSprinting ? ANIMATION_PLAYER_SPEED * 3 / 4 : ANIMATION_PLAYER_SPEED;

    if (IsKeyDown(KEY_D))
    {
        movement.x = speed * deltaTime;
        direction = {1, 0};
        isMoving = true;
    }
    if (IsKeyDown(KEY_A))
    {
        movement.x -= speed * deltaTime;
        direction = {-1, 0};
        isMoving = true;
    }
    if (IsKeyDown(KEY_W))
    {
        movement.y -= speed * deltaTime;
        direction = {0, 1};
        isMoving = true;
    }
    if (IsKeyDown(KEY_S))
    {
        movement.y += speed * deltaTime;
        direction = {0, -1};
        isMoving = true;
    }
}

void Player::handleSprinting(float deltaTime)
{
    if (IsKeyDown(KEY_F) && sptrintcooldownTimer <= 0.0f)
    {
        isSprinting = true;
        sprintTimer = sprintDuration;
        sptrintcooldownTimer = sprintCooldown;
    }

    if (isSprinting)
    {
        sprintTimer -= deltaTime;
        if (sprintTimer <= 0.0f)
            isSprinting = false;
    }

    if (sptrintcooldownTimer > 0.0f)
        sptrintcooldownTimer -= deltaTime;
}

void Player::spawnPlayer(float startx, float starty)
{
    setPos(startx, starty);
    Vector2 gridPos = grid.getGridPosition(startx, starty);
    collisionBox = new Rectangle{position.x - currentTexture.width / 2, position.y - currentTexture.height / 2, (float)currentTexture.width, (float)currentTexture.height};
    collisionID_players++;
    personalCollisionID = collisionID_players;
    collisionMap.addPlayerCollisionRectangle(personalCollisionID, this);
    grid.setCellAttributes(gridPos.x, gridPos.y, 1);
    camera = {0};
    camera.target = (Vector2){position.x, position.y};
    camera.offset = (Vector2){GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 2.25f;
    alive = true;
}

void Player::reloadWeapon()
{
    if (IsKeyPressed(KEY_R) && currentWeapon)
        currentWeapon->reload();
}

void Player::fireWeapon()
{
    if (!currentWeapon)
        return;

    bool fireInput = IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsKeyPressed(KEY_SPACE);
    bool holdInput = IsMouseButtonDown(MOUSE_BUTTON_LEFT) || IsKeyDown(KEY_SPACE);

    bool shouldFire = currentWeapon->isAutomatic() ? holdInput : fireInput;

    if (shouldFire)
        currentWeapon->fire(position, direction);
}

void Player::changeWeapon()
{
    if (!IsKeyPressed(KEY_Q) || weapons.size() <= 1)
        return;

    currentWeaponIndex = (currentWeaponIndex + 1) % static_cast<int>(weapons.size());
    currentWeapon = weapons[currentWeaponIndex];
}

void Player::loadTextures()
{
    leftTexture = LoadTexture("resources/player_left.png");
    leftWalkTexture1 = LoadTexture("resources/player_walk_left_1.png");
    leftWalkTexture2 = LoadTexture("resources/player_walk_left_2.png");
    if (leftTexture.id == 0 || leftWalkTexture1.id == 0 || leftWalkTexture2.id == 0)
    {
        std::cerr << "Error loading player textures" << std::endl;
        exit(1);
    }
}

void Player::updateGridPosition()
{
    Vector2 newGridPos = grid.getGridPosition(position.x, position.y);
    if (newGridPos.x != gridPos.x || newGridPos.y != gridPos.y)
    {
        grid.partialUpdate(position, UPDATE_RADIUS);
        grid.setCellAttributes(gridPos.x, gridPos.y, 0);
        grid.setCellAttributes(newGridPos.x, newGridPos.y, 1);
        gridPos = newGridPos;
    }
}

void Player::updateCamera()
{
    camera.target = (Vector2){position.x, position.y};
}
