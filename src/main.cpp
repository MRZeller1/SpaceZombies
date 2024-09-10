#include "game.h"
#include "player.h"
#include "zombie.h"
#include "bug.h"
#include "gameobject.h"
#include "grid.h"
#include "bullet.h"
#include "pistols.h"
#include "collisionMap.h"
#include "character.h"
#include <iostream>
#include <vector>
#include <raylib.h>

void addZombies(std::vector<Zombie *> &zombies, Grid &grid, CollisionMap &collisionMap, int count);
void addBugs(std::vector<Bug *> &bugs, Grid &grid, CollisionMap &collisionMap, int count);
void addBullets(std::vector<Bullet *> &bullets, Grid &grid, CollisionMap &collisionMap, int count);

int main()
{
        const int screenWidth = GetScreenWidth();
        const int screenHeight = GetScreenHeight();

        int windowWidth = screenWidth * .9;
        int windowHeight = screenHeight * .9;


        // Grid dimensions
        const int width = 3025;
        const int height = 3025;
        // Initialize of the grid with map dimensions and cell size
        Grid grid(width, height, 25);
        CollisionMap collisionMap(3000, 3000);
        // Initialize the window with the grid and title
        Game game(windowWidth, windowHeight, grid, collisionMap, "Space Zombies!!!");
        SetTargetFPS(60);
        // Create bullets for pistols
        std::vector<Bullet *> pistolsBullets;
        addBullets(pistolsBullets, grid, collisionMap, 100);
        Pistols pistols(pistolsBullets);
        // Initialize the players and NPCs
        Pistols* poistolPntr = &pistols;
        Player player(poistolPntr, grid, collisionMap);
        std::vector<Zombie *> zombies;
        std::vector<Bug *> bugs;
        addZombies(zombies, grid, collisionMap, 100);
        addBugs(bugs, grid, collisionMap, 540);

        // Game objects
        game.addObject(new GameObject(300, 250, BLACK, 250.0f, 150.0f, 0, 0, grid, collisionMap));
        game.addObject(new GameObject(0, 0, BLACK, 450.0f, 100.0f, 0, 0, grid, collisionMap));
        game.addObject(new GameObject(400, 500, BLACK, 250.0f, 200.0f, 0, 0, grid, collisionMap));
        game.addObject(new GameObject(0, 500, BLACK, 300.0f, 200.0f, 0, 0, grid, collisionMap));
        game.addObject(new GameObject(650, 500, BLACK, 300.0f, 200.0f, 0, 0, grid, collisionMap));
        game.addObject(new GameObject(800, 100, BLACK, 200.0f, 300.0f, 0, 0, grid, collisionMap));
        game.addObject(new GameObject(600, 100, BLACK, 100.0f, 50.0f, 0, 0, grid, collisionMap));
        game.addObject(new GameObject(0, 0, BLACK, 3000, 25, 0, 0, grid, collisionMap));
        game.addObject(new GameObject(0, 0, BLACK, 25, 3000, 0, 0, grid, collisionMap));
        game.addObject(new GameObject(3000, 0, BLACK, 25, 3000, 0, 0, grid, collisionMap));
        game.addObject(new GameObject(0, 3000, BLACK, 3000, 25, 0, 0, grid, collisionMap));
        game.addObject(new GameObject(2200, 500, BLACK, 300, 50, 0, 0, grid, collisionMap));
        game.addObject(new GameObject(2450, 500, BLACK, 50, 300, 0, 0, grid, collisionMap));

        game.addObject(new GameObject(500, 2200, BLACK, 300, 50, 0, 0, grid, collisionMap));
        game.addObject(new GameObject(500, 1950, BLACK, 50, 300, 0, 0, grid, collisionMap));

        game.addObject(new GameObject(2200, 2450, BLACK, 300, 50, 0, 0, grid, collisionMap));
        game.addObject(new GameObject(2450, 2200, BLACK, 50, 300, 0, 0, grid, collisionMap));
        for (int i = 0; i < 4; i++) {
                game.addObject(new GameObject(800 + i * 425, 1500, BLACK, 75, 75, 0, 0, grid, collisionMap));
        }
        for (int i = 0; i < 4; i++) {
                game.addObject(new GameObject(1500, 800 + i * 425, BLACK, 75, 75, 0, 0, grid, collisionMap));
        }

        // run the game
        game.run(player, zombies, bugs, pistolsBullets);

        return 0;
}
void addZombies(std::vector<Zombie *> &zombies, Grid &grid, CollisionMap &collisionMap, int count)
{
        for (int i = 0; i < count; i++)
        {
                Zombie *zombie = new Zombie(grid, collisionMap);
                zombies.push_back(zombie);
                zombieCount++;
        }
}
void addBugs(std::vector<Bug *> &bugs, Grid &grid, CollisionMap &collisionMap, int count)
{
        for (int i = 0; i < count; i++)
        {
                Bug *bug = new Bug(grid, collisionMap);
                bugs.push_back(bug);
                bugCount++;
        }
}
void addBullets(std::vector<Bullet *> &bullets, Grid &grid, CollisionMap &collisionMap, int count)
{
        for (int i = 0; i < count; i++)
        {
                Bullet *bullet = new Bullet(2, 50, 10, 100, grid, collisionMap);
                bullets.push_back(bullet);
        }
}