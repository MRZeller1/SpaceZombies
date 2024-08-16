#include "window.h"


Window::Window(int width, int height, const Grid &grid, CollisionMap &collisionMap, const char *title)
    : m_width(width), m_height(height), grid(grid), collisionMap(collisionMap),m_title(title)
{
    InitWindow(m_width, m_height, m_title);

    SetTargetFPS(60);
    
}

Window::~Window()
{
    CloseWindow();
}

void Window::addObject(GameObject *object)
{
    objects.push_back(object);
}

std::vector<GameObject *> Window::getObjects()
{
    return objects;
}
void Window::createZombies(std::vector<Zombie *> &zombies)
{
    for (int i = 0; i < Zombie::getZombieCount(); i++)
    {
        Vector2 position = grid.getRandomUnocupiedPosition();
        zombies[i]->spawnZombie(position.x, position.y);
        aliveZombieCount++;
    }
}
void Window::createBugs(std::vector<Bug *> &bugs)
{
    
        Vector2 position = grid.getRandomUnocupiedPosition();
        for(int i = 0; i < 5; i++){
            for(int j = 0; j < 5; j++){
                bugs[i + j]->spawnBug(position.x + i * 5, position.y + j * 5);
                std::cout << "Bug created!" << std::endl;
            }
        }

}
void Window::run(Player &player, std::vector<Zombie *> &zombies, std::vector<Bug *> &bugs)
{

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();
        player.update(deltaTime, objects);
        // zombies[0]->update(deltaTime, objects);
        player.updateCamera();
        BeginDrawing();
        ClearBackground(BLACK);

        DrawText("Space Zombies!", 685, 10, 20, LIGHTGRAY);

        BeginMode2D(player.getCamera());
        DrawRectangle(0, 0, 4000, 4000, DARKGRAY);

        grid.draw();
        if (gameStart == false || aliveZombieCount == 0)
        {
            createZombies(zombies);
            //createBugs(bugs);
            gameStart = true;
        }
        player.draw();
        if (!zombies.empty())
        {
            for (int i = 0; i < zombies.size(); i++)
            {
                if (!zombies[i]->isDead())
                {
                    zombies[i]->update(deltaTime, objects);
                    zombies[i]->draw();
                }
            }
        }
        for (int i = 0; i < bugs.size(); i++)
        {
            std::cout << "Bug update " << bugs[i]->isDead() << std::endl;
            if (!bugs[i]->isDead())
            {
                bugs[i]->update(deltaTime, objects);
                bugs[i]->draw();
            }
        }
       
         //grid.draw();
        for (GameObject *object : objects)
        {
            object->draw();
        }

        EndMode2D();
        EndDrawing();
    }
}