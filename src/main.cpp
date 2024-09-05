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

int main()
{
        // Window dimensions
        const int width = 800;
        const int height = 500;
        // Initialize of the grid with map dimensions and cell size
        Grid grid(width, height, 50);
        CollisionMap collisionMap(4000, 4000);
        // Initialize the window with the grid and title
        Game game(1000, 1000, grid, collisionMap, "Space Zombies!!!");
        SetTargetFPS(60);
        std::vector<Bullet *> pistolsBullets;
        std::vector<Bullet *> rifleBullets;
        std::vector<Bullet *> flamethrowerBullets;

        for (int i = 0; i < 40; i++)
        {
                Bullet *bullet = new Bullet(2, 50, 10, 100, grid, collisionMap);
                pistolsBullets.push_back(bullet);
        }
        
        Pistols pistols(pistolsBullets);
        // Initialize the player object
        Player player(pistols, grid, collisionMap);
        std::vector<Zombie *> zombies;
        std::vector<Bug *> bugs;
        for (int i = 0; i < 5; i++)
        {
                Zombie *zombie = new Zombie(grid, collisionMap);
                zombies.push_back(zombie);
                zombieCount++;
        }
        for (int i = 0; i < 0; i++)
        {
                Bug *bug = new Bug(grid, collisionMap);
                bugs.push_back(bug);
                bugCount++;
        }
        std::cout << "Zombie count: " << zombies.size() << std::endl;

        // Game objects
        GameObject *block = new GameObject(300, 250, BLACK, 250.0f, 150.0f, 0, 0, grid, collisionMap);
        GameObject *block1 = new GameObject(0, 0, BLACK, 450.0f, 100.0f, 0, 0, grid, collisionMap);

        GameObject *block2 = new GameObject(400, 500, BLACK, 250.0f, 200.0f, 0, 0, grid, collisionMap);
        GameObject *block3 = new GameObject(0, 500, BLACK, 300.0f, 200.0f, 0, 0, grid, collisionMap);
        GameObject *block4 = new GameObject(650, 500, BLACK, 300.0f, 200.0f, 0, 0, grid, collisionMap);
        GameObject *block5 = new GameObject(800, 100, BLACK, 200.0f, 300.0f, 0, 0, grid, collisionMap);
        GameObject *block6 = new GameObject(600, 100, BLACK, 100.0f, 50.0f, 0, 0, grid, collisionMap);

        // adding objects to the window
        game.addObject(block);
        game.addObject(block1);

        game.addObject(block2);
        game.addObject(block3);
        game.addObject(block4);
        game.addObject(block5);
        game.addObject(block6);

        // initialize the grid distances/directions
        // grid.getGridNode(200, 200)->setAttributes(1);
        // grid.getGridNode(200, 200)->update();

        // run the game
        game.run(player, zombies, bugs, pistolsBullets);

        return 0;
}