#include "game.h"
#include "player.h"
#include "zombie.h"
#include "bug.h"
#include "gameobject.h"
#include "grid.h"
#include "bullet.h"
#include "weapon.h"
#include "textures.h"
#include "collisionMap.h"
#include "character.h"
#include <iostream>
#include <vector>
#include <raylib.h>

void addZombies(std::vector<Zombie *> &zombies, Grid &grid, CollisionMap &collisionMap, int count);
void addBugs(std::vector<Bug *> &bugs, Grid &grid, CollisionMap &collisionMap, int count);
void addBullets(std::vector<Bullet *> &bullets, Grid &grid, CollisionMap &collisionMap, int count);
void setupArena(Game &game, Grid &grid, CollisionMap &collisionMap);

namespace
{
const Color HULL_FILL = {32, 30, 34, 255};
const Color HULL_BORDER = {72, 68, 64, 255};
const Color POD_FILL = {48, 42, 36, 255};
const Color POD_BORDER = {88, 78, 62, 255};
const Color BULK_FILL = {38, 36, 40, 255};
const Color BULK_BORDER = {68, 64, 60, 255};
const Color REACTOR_FILL = {42, 38, 36, 255};
const Color REACTOR_BORDER = {110, 75, 40, 255};
const Color CRATE_FILL = {52, 46, 40, 255};
const Color CRATE_BORDER = {82, 72, 58, 255};

void addHull(Game &game, Grid &grid, CollisionMap &collisionMap, float x, float y, float w, float h)
{
    game.addObject(new GameObject(x, y, HULL_FILL, w, h, 0, 0, grid, collisionMap, HULL_BORDER, ObstacleType::Hull));
}

void addPod(Game &game, Grid &grid, CollisionMap &collisionMap, float x, float y, float w, float h)
{
    game.addObject(new GameObject(x, y, POD_FILL, w, h, 0, 0, grid, collisionMap, POD_BORDER, ObstacleType::SupplyPod));
}

void addBulk(Game &game, Grid &grid, CollisionMap &collisionMap, float x, float y, float w, float h)
{
    game.addObject(new GameObject(x, y, BULK_FILL, w, h, 0, 0, grid, collisionMap, BULK_BORDER, ObstacleType::Bulkhead));
}

void addReactor(Game &game, Grid &grid, CollisionMap &collisionMap, float x, float y, float size)
{
    game.addObject(new GameObject(x, y, REACTOR_FILL, size, size, 0, 0, grid, collisionMap, REACTOR_BORDER, ObstacleType::ReactorPillar));
}

void addCrate(Game &game, Grid &grid, CollisionMap &collisionMap, float x, float y, float w, float h)
{
    game.addObject(new GameObject(x, y, CRATE_FILL, w, h, 0, 0, grid, collisionMap, CRATE_BORDER, ObstacleType::CargoCrate));
}
}

void setupArena(Game &game, Grid &grid, CollisionMap &collisionMap)
{
    const float M = 1975.0f;
    const float W = 25.0f;

    // Outer hull
    addHull(game, grid, collisionMap, 0, 0, M, W);
    addHull(game, grid, collisionMap, 0, 0, W, M);
    addHull(game, grid, collisionMap, M, 0, W, M);
    addHull(game, grid, collisionMap, 0, M, M, W);

    // Command core — reactor pillars with open lanes between them
    addReactor(game, grid, collisionMap, 870, 870, 60);
    addReactor(game, grid, collisionMap, 1070, 870, 60);
    addReactor(game, grid, collisionMap, 870, 1070, 60);
    addReactor(game, grid, collisionMap, 1070, 1070, 60);

    // Short hull ribs along outer rooms (cover only, never seal an area)
    addHull(game, grid, collisionMap, 120, 120, 180, W);
    addHull(game, grid, collisionMap, 120, 120, W, 160);
    addHull(game, grid, collisionMap, 1680, 120, 180, W);
    addHull(game, grid, collisionMap, 1830, 120, W, 160);
    addHull(game, grid, collisionMap, 120, 1680, 180, W);
    addHull(game, grid, collisionMap, 120, 1720, W, 160);
    addHull(game, grid, collisionMap, 1680, 1680, 180, W);
    addHull(game, grid, collisionMap, 1830, 1680, W, 160);

    // NW cargo — crates hug the corner, wide lane into the cross
    addCrate(game, grid, collisionMap, 200, 200, 80, 70);
    addCrate(game, grid, collisionMap, 380, 180, 75, 75);
    addPod(game, grid, collisionMap, 260, 380, 70, 65);
    addCrate(game, grid, collisionMap, 520, 300, 70, 70);

    // NE engine — spaced machinery, open floor to the hub
    addReactor(game, grid, collisionMap, 1580, 200, 55);
    addReactor(game, grid, collisionMap, 1750, 220, 55);
    addBulk(game, grid, collisionMap, 1650, 380, 140, 30);
    addPod(game, grid, collisionMap, 1780, 420, 65, 65);

    // SW maintenance
    addBulk(game, grid, collisionMap, 200, 1620, 30, 140);
    addCrate(game, grid, collisionMap, 300, 1720, 75, 70);
    addCrate(game, grid, collisionMap, 480, 1680, 70, 75);
    addPod(game, grid, collisionMap, 360, 1850, 65, 65);

    // SE armory — scattered cover, no enclosed pocket
    addCrate(game, grid, collisionMap, 1620, 1720, 80, 80);
    addCrate(game, grid, collisionMap, 1780, 1780, 75, 75);
    addPod(game, grid, collisionMap, 1700, 1580, 70, 70);
    addCrate(game, grid, collisionMap, 1850, 1650, 70, 70);

    // Wing flanking cover — sits off the main N/S and E/W lanes
    addPod(game, grid, collisionMap, 860, 420, 65, 65);
    addPod(game, grid, collisionMap, 1110, 420, 65, 65);
    addPod(game, grid, collisionMap, 860, 1510, 65, 65);
    addPod(game, grid, collisionMap, 1110, 1510, 65, 65);

    addCrate(game, grid, collisionMap, 420, 860, 70, 70);
    addCrate(game, grid, collisionMap, 420, 1110, 70, 70);
    addCrate(game, grid, collisionMap, 1510, 860, 70, 70);
    addCrate(game, grid, collisionMap, 1510, 1110, 70, 70);
}

int main()
{
        ChangeDirectory(GetApplicationDirectory());

        const int screenWidth = GetScreenWidth();
        const int screenHeight = GetScreenHeight();

        int windowWidth = screenWidth * .9;
        int windowHeight = screenHeight * .9;

        const int mapSize = 2000;
        const int playBounds = 1975;
        Grid grid(mapSize, mapSize, 25);
        CollisionMap collisionMap(playBounds, playBounds);
        Game game(windowWidth, windowHeight, grid, collisionMap, "Space Zombies!!!");
        SetTargetFPS(60);
        GameTextures::Load();

        std::vector<Bullet *> bullets;
        addBullets(bullets, grid, collisionMap, 40);
        Weapon pistol(WeaponType::Pistol, bullets);
        Weapon rifle(WeaponType::Rifle, bullets);
        Weapon flamethrower(WeaponType::Flamethrower, bullets);
        std::vector<Weapon *> weapons = {&pistol, &rifle, &flamethrower};
        Player player(weapons, grid, collisionMap);
        std::vector<Zombie *> zombies;
        std::vector<Bug *> bugs;
        addZombies(zombies, grid, collisionMap, 20);
        addBugs(bugs, grid, collisionMap, 18);

        setupArena(game, grid, collisionMap);
        player.spawnAtClearPosition(400.0f, 1000.0f);

        game.run(player, zombies, bugs, bullets);

        GameTextures::Unload();
        return 0;
}

void addZombies(std::vector<Zombie *> &zombies, Grid &grid, CollisionMap &collisionMap, int count)
{
        for (int i = 0; i < count; i++)
        {
                Zombie *zombie = new Zombie(grid, collisionMap);
                zombies.push_back(zombie);
        }
}
void addBugs(std::vector<Bug *> &bugs, Grid &grid, CollisionMap &collisionMap, int count)
{
        for (int i = 0; i < count; i++)
        {
                Bug *bug = new Bug(grid, collisionMap);
                bugs.push_back(bug);
        }
}
void addBullets(std::vector<Bullet *> &bullets, Grid &grid, CollisionMap &collisionMap, int count)
{
        for (int i = 0; i < count; i++)
        {
                Bullet *bullet = new Bullet(3, 420, 25, 1.2f, grid, collisionMap);
                bullets.push_back(bullet);
        }
}
