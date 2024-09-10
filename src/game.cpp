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
    : m_width(width), m_height(height), grid(grid), collisionMap(collisionMap), m_title(title), score(0), lastScoreUpdate(0)
{
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    int windowPosX = (screenWidth - width) * .05;
    int windowPosY = (screenWidth - height) * .05;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(width, height, m_title);
    SetWindowPosition(windowPosX, windowPosY);

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
        position.x += 12;
        position.y += 12;
        zombies[i]->spawnZombie(position.x, position.y);
        aliveZombieCount++;
    }
}
void Game::createBugs(std::vector<Bug *> &bugs) {
    const int gridSize = 3;
    const int bugSize = 4;
    const int spacing = bugSize + 1; // 1 pixel gap between bugs

    for (int k = 0; k < 60; k++) { // Create 5 groups of bug grids
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
        updateScore(deltaTime);
        player.update(deltaTime, objects);
        player.updateCamera();
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("Space Zombies!", 685, 10, 20, LIGHTGRAY);
        BeginMode2D(player.getCamera());
        DrawRectangle(0, 0, 3025, 3025, DARKGRAY);

        //grid.draw();
        if (gameStart == false || aliveZombieCount == 0)
        {
            createZombies(zombies);
            createBugs(bugs);
            gameStart = true;
            //aliveZombieCount++;
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
        for (GameObject *object : objects)
        {
            object->draw();
        }
        
        EndMode2D();
        
        if (player.isDead()) {
            drawGameOverBanner(player);
            player.setHealth(0);
        }else{
            drawScore();
            finalScore = score;
        }
        drawHealthBar(player);

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

void Game::drawHealthBar(const Player &player)
{
    int barWidth = 200;
    int barHeight = 20;
    int borderSize = 2;
    int padding = 10; 
    
    // Draw border
    DrawRectangle(padding, padding, barWidth + borderSize * 2, barHeight + borderSize * 2, WHITE);
    
    // Draw background
    DrawRectangle(padding + borderSize, padding + borderSize, barWidth, barHeight, RED);
    
    // Draw health
    int healthWidth = static_cast<int>((player.getHealth() / static_cast<float>(DEFAULT_PLAYER_HEALTH)) * barWidth);
    DrawRectangle(padding + borderSize, padding + borderSize, healthWidth, barHeight, GREEN);
    
    // Draw text
    std::string healthText = std::to_string(player.getHealth()) + " / " + std::to_string(DEFAULT_PLAYER_HEALTH);
    DrawText(healthText.c_str(), padding + 5, padding + 2, 16, WHITE);
}

void Game::drawGameOverBanner(Player &player)
{
    const char* gameOverText = "Game Over";
    const char* scoreText = TextFormat("Score: %.0f", finalScore);
    int fontSize = 40;
    int smallFontSize = 30;
    
    int gameOverTextWidth = MeasureText(gameOverText, fontSize);
    int scoreTextWidth = MeasureText(scoreText, smallFontSize);
    int textHeight = fontSize;

    int rectWidth = std::max(gameOverTextWidth, scoreTextWidth) + 40;
    int rectHeight = textHeight * 2 + smallFontSize + 60;

    // Get player's position on screen
    Vector2 playerScreenPos = GetWorldToScreen2D(player.getPosition(), player.getCamera());

    // Calculate banner position
    int bannerX = playerScreenPos.x - rectWidth / 2;
    int bannerY = playerScreenPos.y - player.getTextureHeight() / 2 - 200 - rectHeight / 2;

    // Draw the background rectangle
    DrawRectangle(bannerX, bannerY, rectWidth, rectHeight, Color{0, 0, 0, 200});

    // Draw the "Game Over" text
    DrawText(gameOverText, bannerX + (rectWidth - gameOverTextWidth) / 2, 
             bannerY + 20, fontSize, RED);

    // Draw the score text
    DrawText(scoreText, bannerX + (rectWidth - scoreTextWidth) / 2, 
             bannerY + rectHeight - smallFontSize - 20, smallFontSize, WHITE);
}



void Game::updateScore(float deltaTime) 
{
    lastScoreUpdate += deltaTime;
    if (lastScoreUpdate >= 1.0f) {
        score += 1;
        lastScoreUpdate -= 1.0f;
    }
}

void Game::drawScore() {
    const char* scoreText = TextFormat("Score: %.0f", score);
    int fontSize = 20;
    int padding = 10;
    
    int textWidth = MeasureText(scoreText, fontSize);
    int textHeight = fontSize;
    
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    
    // Draw background rectangle
    DrawRectangle(screenWidth - textWidth - padding * 2, 0, textWidth + padding * 2, textHeight + padding * 2, Color{0, 0, 0, 150});
    
    // Draw score text
    DrawText(scoreText, screenWidth - textWidth - padding, padding, fontSize, WHITE);
}