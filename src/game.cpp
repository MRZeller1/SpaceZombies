#include "game.h"
#include "raylib.h"
#include "player.h"
#include "zombie.h"
#include "bug.h"
#include "bullet.h"
#include "collisionMap.h"
#include "grid.h"
#include "gameobject.h"

Game::Game(int width, int height, const Grid &grid, CollisionMap &collisionMap, const char *title)
    : m_width(width), m_height(height), grid(grid), collisionMap(collisionMap), m_title(title)
{
    InitWindow(m_width, m_height, m_title);

    SetTargetFPS(60);
}

Game::~Game()
{
    CloseWindow();
}

void Game::addObject(GameObject *object)
{
    objects.push_back(object);
}

std::vector<GameObject *> Game::getObjects()
{
    return objects;
}
void Game::createZombies(std::vector<Zombie *> &zombies)
{
    for (int i = 0; i < Zombie::getZombieCount(); i++)
    {
        Vector2 position = grid.getRandomUnocupiedPosition();
        zombies[i]->spawnZombie(position.x, position.y);
        aliveZombieCount++;
    }
}
void Game::createBugs(std::vector<Bug *> &bugs) {
    const int gridSize = 5;
    const int bugSize = 4;
    const int spacing = bugSize + 1; // 1 pixel gap between bugs

    for (int k = 0; k < 20; k++) { // Create 5 groups of bug grids
        Vector2 startPosition = grid.getRandomUnocupiedPosition();
        for (int i = 0; i < gridSize; i++) {
            for (int j = 0; j < gridSize; j++) {
                int index = k * gridSize * gridSize + i * gridSize + j;
                if (index < bugs.size()) {
                    float x = startPosition.x + i * spacing;
                    float y = startPosition.y + j * spacing;
                    bugs[index]->spawnBug(x, y);
                }
            }
        }
    }
}
void Game::run(Player &player, std::vector<Zombie *> &zombies, std::vector<Bug *> &bugs, std::vector<Bullet *> &bullets)
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
            //createZombies(zombies);
            createBugs(bugs);
            gameStart = true;
            aliveZombieCount++;
        }
        player.draw();

        
        if (!zombies.empty())
        {
            updateZombies(zombies, deltaTime);
        }else{
            createZombies(zombies);
        }
        updateBugs(bugs, deltaTime);
        updateBullets(bullets, deltaTime);
        // grid.draw();
        for (GameObject *object : objects)
        {
            object->draw();
        }
        if(player.isDead()){
            gameOver = true;
            DrawRectangle(player.getX() - 68, player.getY() - 129, 120, 100, DARKGRAY);
            DrawText("Game Over", player.getX() - 60, player.getY() - 120, 20, RED);
        }
        //drawBullets(player);
        EndMode2D();
        EndDrawing();
    }
}

void Game::updateBullets(std::vector<Bullet*> &bullets, float deltaTime)
{
    for (int i = 0; i < bullets.size(); i++)
    {
        if (bullets[i]->isActive())
        {
            bullets[i]->update(deltaTime);
            bullets[i]->draw();
        }
    }
}

void Game::updateZombies(std::vector<Zombie *> &zombies, float deltaTime)
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

void Game::updateBugs(std::vector<Bug *> &bugs, float deltaTime)
{
    for (int i = 0; i < bugs.size(); i++)
    {
        if (!bugs[i]->isDead())
        {
            bugs[i]->update(deltaTime, objects);
            bugs[i]->draw();
        }
    }
}

void Game::drawHealthBar(Player &player)
{
    DrawRectangle(player.getX(), player.getY(), 200, 20, BLACK);
    DrawRectangle(player.getX() - (player.getTextureWidth() / 2), player.getY(), player.getHealth() * 2, 20, GREEN);
    
}