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
const Color HULL_FILL = {36, 42, 58, 255};
const Color HULL_BORDER = {55, 200, 220, 255};
const Color POD_FILL = {48, 92, 118, 255};
const Color POD_BORDER = {90, 210, 240, 255};
const Color BULK_FILL = {44, 52, 68, 255};
const Color BULK_BORDER = {78, 168, 190, 255};
const Color REACTOR_FILL = {52, 62, 82, 255};
const Color REACTOR_BORDER = {100, 220, 245, 255};
const Color ROCK_FILL = {86, 84, 80, 255};
const Color ROCK_BORDER = {64, 62, 58, 255};

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

void addRock(Game &game, Grid &grid, CollisionMap &collisionMap, float x, float y, float size)
{
    game.addObject(new GameObject(x, y, ROCK_FILL, size, size, 0, 0, grid, collisionMap, ROCK_BORDER, ObstacleType::MoonRock));
}
}

void setupArena(Game &game, Grid &grid, CollisionMap &collisionMap)
{
    const float M = 1975.0f;
    const float W = 25.0f;

    // Landing pad perimeter fence
    addHull(game, grid, collisionMap, 0, 0, M, W);
    addHull(game, grid, collisionMap, 0, 0, W, M);
    addHull(game, grid, collisionMap, M, 0, W, M);
    addHull(game, grid, collisionMap, 0, M, M, W);

    // Ship cross — outer hull shell
    addHull(game, grid, collisionMap, 775, 775, 450, W);   // core N edge
    addHull(game, grid, collisionMap, 775, 1200, 450, W);  // core S edge
    addHull(game, grid, collisionMap, 775, 775, W, 450);   // core W edge
    addHull(game, grid, collisionMap, 1200, 775, W, 450);  // core E edge

    addHull(game, grid, collisionMap, 850, 25, 300, W);    // north wing rim
    addHull(game, grid, collisionMap, 850, 750, 300, W);
    addHull(game, grid, collisionMap, 850, 1225, 300, W);
    addHull(game, grid, collisionMap, 850, 1925, 300, W);

    addHull(game, grid, collisionMap, 25, 850, W, 300);     // west wing rim
    addHull(game, grid, collisionMap, 750, 850, W, 300);
    addHull(game, grid, collisionMap, 1225, 850, W, 300);
    addHull(game, grid, collisionMap, 1925, 850, W, 300);

    // Airlock gaps at wing tips (openings already between hull segments)

    // --- Command core: reactor pillars (kept!) ---
    addReactor(game, grid, collisionMap, 860, 860, 65);
    addReactor(game, grid, collisionMap, 1075, 860, 65);
    addReactor(game, grid, collisionMap, 860, 1075, 65);
    addReactor(game, grid, collisionMap, 1075, 1075, 65);
    addBulk(game, grid, collisionMap, 940, 820, 120, 35);
    addBulk(game, grid, collisionMap, 940, 1145, 120, 35);

    // --- NW moon surface (exterior) ---
    addRock(game, grid, collisionMap, 120, 100, 90);
    addRock(game, grid, collisionMap, 350, 180, 110);
    addRock(game, grid, collisionMap, 180, 420, 75);
    addRock(game, grid, collisionMap, 550, 500, 95);
    addPod(game, grid, collisionMap, 280, 300, 80, 70);   // supply drop on moon

    // --- NE moon surface ---
    addRock(game, grid, collisionMap, 1550, 120, 100);
    addRock(game, grid, collisionMap, 1750, 300, 85);
    addRock(game, grid, collisionMap, 1400, 450, 110);
    addRock(game, grid, collisionMap, 1680, 550, 70);
    addPod(game, grid, collisionMap, 1500, 250, 90, 75);

    // --- SW moon surface ---
    addRock(game, grid, collisionMap, 100, 1450, 105);
    addRock(game, grid, collisionMap, 400, 1650, 90);
    addRock(game, grid, collisionMap, 250, 1750, 80);
    addRock(game, grid, collisionMap, 600, 1400, 100);
    addPod(game, grid, collisionMap, 180, 1580, 85, 80);

    // --- SE moon surface ---
    addRock(game, grid, collisionMap, 1580, 1500, 95);
    addRock(game, grid, collisionMap, 1750, 1700, 110);
    addRock(game, grid, collisionMap, 1420, 1750, 85);
    addRock(game, grid, collisionMap, 1650, 1350, 75);

    // --- Ship interior cover (wings) ---
    addBulk(game, grid, collisionMap, 900, 180, 200, 40);
    addBulk(game, grid, collisionMap, 920, 280, 40, 180);
    addPod(game, grid, collisionMap, 1050, 350, 75, 75);

    addBulk(game, grid, collisionMap, 900, 1700, 200, 40);
    addPod(game, grid, collisionMap, 1100, 1620, 80, 80);

    addBulk(game, grid, collisionMap, 120, 920, 40, 200);
    addPod(game, grid, collisionMap, 280, 1050, 75, 75);

    addBulk(game, grid, collisionMap, 1750, 940, 40, 200);
    addBulk(game, grid, collisionMap, 1600, 1050, 120, 40);
    addPod(game, grid, collisionMap, 1680, 880, 70, 70);
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
