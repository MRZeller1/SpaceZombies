#include "window.h"
#include "player.h"
#include "zombie.h"
#include "bug.h"
#include "gameobject.h"
#include "grid.h"
#include "bullet.h"

int main()
{
        // Window dimensions
        const int width = 800;
        const int height = 500;
        // Initialize of the grid with map dimensions and cell size
        Grid grid(width, height, 50);
        CollisionMap collisionMap(4000, 4000);
        // Initialize the window with the grid and title
        Window window(1000, 1000, grid, collisionMap, "Space Zombies!!!");
        SetTargetFPS(60);
        // Initialize the player object
        Player player(grid, collisionMap);
        std::vector<Zombie *> zombies;
        std::vector<Bug*> bugs;
        for(int i = 0; i < 5; i++){
            Zombie *zombie = new Zombie(grid, collisionMap);
            zombies.push_back(zombie);
            zombieCount++;
        }
        for(int i = 0; i < 0; i++){
            Bug *bug = new Bug(grid, collisionMap);
            bugs.push_back(bug);
            bugCount++;
        }
        std::cout << "Zombie count: " << zombies.size() << std::endl;

        std::vector<Bullet*> pistolsBullets;
        std::vector<Bullet*> rifleBullets;
        std::vector<Bullet*> flamethrowerBullets;

        for(int i = 0; i < 20; i++){
                Bullet* bullet = new Bullet(2, 50, 10, 100, grid, collisionMap);
                pistolsBullets.push_back(bullet);
        }
        for(int i = 0; i < 30; i++){
                Bullet* bullet = new Bullet(2, 50, 10, 100, grid, collisionMap);
                rifleBullets.push_back(bullet);
        }
        for(int i = 0; i < 10; i++){
                Bullet* bullet = new Bullet(50, 50, 10, 100, grid, collisionMap);
                flamethrowerBullets.push_back(bullet);
        }

        // Game objects
        GameObject *block = new GameObject(300, 250, BLACK, 250.0f, 150.0f, 0, 0, grid, collisionMap);
        GameObject *block1 = new GameObject(0, 0, BLACK, 450.0f, 100.0f, 0, 0, grid, collisionMap);

        GameObject *block2 = new GameObject(400, 500, BLACK, 250.0f, 200.0f, 0, 0, grid, collisionMap);
        GameObject *block3 = new GameObject(0, 500, BLACK, 300.0f, 200.0f, 0, 0, grid, collisionMap);
        GameObject *block4 = new GameObject(650, 500, BLACK, 300.0f, 200.0f, 0, 0, grid, collisionMap);
        GameObject *block5 = new GameObject(800, 100, BLACK, 200.0f, 300.0f, 0, 0, grid, collisionMap);
        GameObject *block6 = new GameObject(600, 100, BLACK, 100.0f, 50.0f, 0, 0, grid, collisionMap);

        // adding objects to the window
        window.addObject(block);
        window.addObject(block1);

        window.addObject(block2);
        window.addObject(block3);
        window.addObject(block4);
        window.addObject(block5);
        window.addObject(block6);

        // initialize the grid distances/directions
        // grid.getGridNode(200, 200)->setAttributes(1);
        // grid.getGridNode(200, 200)->update();

        // run the game
        window.run(player, zombies, bugs);

        return 0;
}