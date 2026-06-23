#include "game.h"
#include "raylib.h"
#include "player.h"
#include "zombie.h"
#include "bug.h"
#include "bullet.h"
#include "collisionMap.h"
#include "grid.h"
#include "gameobject.h"
#include "weapon.h"

Game::Game(int width, int height, const Grid &grid, CollisionMap &collisionMap, const char *title)
    : m_width(width), m_height(height), grid(grid), collisionMap(collisionMap), m_title(title), score(0),
      lastScoreUpdate(0), viewportW(width), viewportH(height)
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(width, height, m_title);
    PollInputEvents();

    const int monitorW = GetMonitorWidth(GetCurrentMonitor());
    const int monitorH = GetMonitorHeight(GetCurrentMonitor());
    updateViewport();
    SetWindowPosition((monitorW - viewportW) / 2, (monitorH - viewportH) / 2);

    SetTargetFPS(60);
}

void Game::updateViewport()
{
    if (GetScreenWidth() > 0)
        viewportW = GetScreenWidth();
    if (GetScreenHeight() > 0)
        viewportH = GetScreenHeight();
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
void Game::createZombies(std::vector<Zombie *> &zombies, Vector2 playerPos)
{
    const float minSpawnDistance = 280.0f;
    aliveZombieCount = 0;

    for (size_t i = 0; i < zombies.size(); i++)
    {
        if (zombies[i] == nullptr)
            continue;

        if (!zombies[i]->isDead())
        {
            aliveZombieCount++;
            continue;
        }

        Vector2 position = grid.getClearSpawnPosition(
            collisionMap,
            (float)zombies[i]->getTextureWidth(),
            (float)zombies[i]->getTextureHeight(),
            playerPos.x, playerPos.y,
            playerPos.x, playerPos.y, minSpawnDistance);
        zombies[i]->spawnZombie(position.x, position.y);
        aliveZombieCount++;
    }
}

void Game::createBugs(std::vector<Bug *> &bugs, Vector2 playerPos)
{
    const int gridSize = 3;
    const int spacing = 5;
    const int bugsPerGroup = gridSize * gridSize;
    const int groupCount = static_cast<int>(bugs.size()) / bugsPerGroup;
    const float minSpawnDistance = 220.0f;

    for (int k = 0; k < groupCount; k++)
    {
        bool groupAlive = false;
        for (int n = 0; n < bugsPerGroup; n++)
        {
            int idx = k * bugsPerGroup + n;
            if (idx < static_cast<int>(bugs.size()) && bugs[idx] && !bugs[idx]->isDead())
            {
                groupAlive = true;
                break;
            }
        }
        if (groupAlive)
            continue;

        Vector2 startPosition = grid.getClearGroupSpawnPosition(
            collisionMap,
            (float)bugs[0]->getTextureWidth(),
            (float)bugs[0]->getTextureHeight(),
            gridSize, spacing,
            playerPos.x, playerPos.y, minSpawnDistance);
        for (int i = 0; i < gridSize; i++)
        {
            for (int j = 0; j < gridSize; j++)
            {
                int index = k * bugsPerGroup + i * gridSize + j;
                if (index < static_cast<int>(bugs.size()) && bugs[index])
                    bugs[index]->spawnBug(startPosition.x + i * spacing, startPosition.y + j * spacing);
            }
        }
    }
}
void Game::run(Player &player, std::vector<Zombie *> &zombies, std::vector<Bug *> &bugs, std::vector<Bullet *> &bullets)
{

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();
        updateViewport();
        updateScore(deltaTime);
        player.update(deltaTime, objects);

        if (!gameStart || countAliveZombies(zombies) == 0)
        {
            createZombies(zombies, player.getPosition());
            createBugs(bugs, player.getPosition());
            gameStart = true;
        }

        if (!zombies.empty())
            updateZombies(zombies, deltaTime);
        updateBugs(bugs, deltaTime);
        updateBullets(bullets, deltaTime);

        player.updateCamera(viewportW, viewportH);

        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(player.getCamera());
        drawEnvironment();
        player.draw();
        if (Weapon *weapon = player.getWeapon())
            weapon->draw(player.getPosition(), player.getDirection());

        for (int i = 0; i < static_cast<int>(zombies.size()); i++)
        {
            if (zombies[i] && !zombies[i]->isDead())
                zombies[i]->draw();
        }
        for (int i = 0; i < static_cast<int>(bugs.size()); i++)
        {
            if (bugs[i] && !bugs[i]->isDead())
                bugs[i]->draw();
        }
        for (int i = 0; i < static_cast<int>(bullets.size()); i++)
        {
            if (bullets[i] && bullets[i]->isActive())
                bullets[i]->draw();
        }
        for (GameObject *object : objects)
            object->draw();

        EndMode2D();

        BeginScissorMode(0, 0, viewportW, viewportH);
        if (player.isDead())
        {
            drawGameOverBanner(player);
            player.setHealth(0);
        }
        else
        {
            finalScore = score;
            drawScreenHud(player);
        }
        EndScissorMode();

        EndDrawing();
    }
}

void Game::updateBullets(std::vector<Bullet*> &bullets, float deltaTime)
{
    for (int i = 0; i < static_cast<int>(bullets.size()); i++)
    {
        if (bullets[i] && bullets[i]->isActive())
            bullets[i]->update(deltaTime);
    }
}

void Game::updateZombies(std::vector<Zombie *> &zombies, float deltaTime)
{
    for (int i = 0; i < static_cast<int>(zombies.size()); i++)
    {
        if (zombies[i] && !zombies[i]->isDead())
            zombies[i]->update(deltaTime, objects);
    }
}

void Game::updateBugs(std::vector<Bug *> &bugs, float deltaTime)
{
    for (int i = 0; i < static_cast<int>(bugs.size()); i++)
    {
        if (bugs[i] && !bugs[i]->isDead())
            bugs[i]->update(deltaTime, objects);
    }
}

void Game::drawWeaponHud(const Player &player)
{
    const Weapon *weapon = player.getWeapon();
    if (!weapon)
        return;

    const int padding = 12;
    const int bulletW = 8;
    const int bulletH = 16;
    const int bulletGap = 4;
    const int maxAmmo = weapon->getMaxAmmo();
    const int panelWidth = maxAmmo * bulletW + (maxAmmo - 1) * bulletGap + padding * 2;
    const int panelHeight = 56;
    const int panelX = viewportW - panelWidth - padding;
    const int panelY = viewportH - panelHeight - padding;

    DrawRectangle(panelX, panelY, panelWidth, panelHeight, Color{0, 0, 0, 170});
    DrawRectangleLines(panelX, panelY, panelWidth, panelHeight, Color{180, 180, 180, 255});

    const char *weaponName = weapon->getName();
    int nameWidth = MeasureText(weaponName, 16);
    DrawText(weaponName, panelX + (panelWidth - nameWidth) / 2, panelY + 6, 16, LIGHTGRAY);

    const int bulletsY = panelY + 28;
    const int bulletsStartX = panelX + padding;
    const int currentAmmo = weapon->getAmmo();
    const Color loadedColor = weapon->isReloading() ? Color{220, 140, 40, 255} : Color{230, 210, 80, 255};
    const Color casingColor = Color{120, 95, 50, 255};
    const Color emptyColor = Color{50, 50, 50, 255};

    for (int i = 0; i < maxAmmo; i++)
    {
        int x = bulletsStartX + i * (bulletW + bulletGap);
        bool loaded = i < currentAmmo;
        DrawRectangle(x, bulletsY, bulletW, bulletH, loaded ? casingColor : emptyColor);
        DrawRectangle(x + 1, bulletsY + 1, bulletW - 2, 5, loaded ? loadedColor : Color{70, 70, 70, 255});
        if (loaded)
            DrawRectangle(x + 2, bulletsY + bulletH - 4, bulletW - 4, 2, Color{180, 160, 70, 255});
    }

    const char *ammoText = weapon->isReloading()
                               ? "Reloading..."
                               : TextFormat("%d / %d", currentAmmo, maxAmmo);
    int ammoWidth = MeasureText(ammoText, 12);
    DrawText(ammoText, panelX + (panelWidth - ammoWidth) / 2, panelY + panelHeight - 16, 12, WHITE);
}

void Game::drawScreenHud(Player &player)
{
    const char *title = "Space Zombies!";
    int titleWidth = MeasureText(title, 20);
    DrawText(title, (viewportW - titleWidth) / 2, 10, 20, LIGHTGRAY);

    drawScore();
    drawPlayerHud(player);
    drawWeaponHud(player);
}

int Game::countAliveZombies(const std::vector<Zombie *> &zombies) const
{
    int alive = 0;
    for (Zombie *zombie : zombies)
    {
        if (zombie && !zombie->isDead())
            alive++;
    }
    return alive;
}

void Game::drawEnvironment()
{
    const int mapW = grid.getMapWidth();
    const int mapH = grid.getMapHeight();
    const int tile = 50;

    const Color deckA = {22, 22, 26, 255};
    const Color deckB = {18, 18, 22, 255};

    for (int ty = 0; ty < mapH; ty += tile)
    {
        for (int tx = 0; tx < mapW; tx += tile)
        {
            bool alt = ((tx / tile) + (ty / tile)) % 2 == 0;
            DrawRectangle(tx, ty, tile, tile, alt ? deckA : deckB);
        }
    }

    // Worn panel seams across the hull
    for (int ty = 0; ty < mapH; ty += 80)
        DrawLineEx({0, (float)ty}, {(float)mapW, (float)ty}, 1.0f, Color{12, 12, 14, 140});
    for (int tx = 0; tx < mapW; tx += 80)
        DrawLineEx({(float)tx, 0}, {(float)tx, (float)mapH}, 1.0f, Color{12, 12, 14, 140});

    // Grime patches
    const Vector2 stains[] = {{280, 340}, {620, 180}, {1580, 420}, {1750, 1550}, {420, 1680}, {1380, 1720}};
    for (Vector2 s : stains)
        DrawCircleV(s, 55, Color{14, 13, 16, 90});

    // Main corridor lanes (open cross, ~260px wide arms)
    DrawRectangle(870, 0, 260, mapH, Color{26, 25, 28, 70});
    DrawRectangle(0, 870, mapW, 260, Color{26, 25, 28, 70});

    // Reactor chamber floor ring
    DrawCircleLines(1000, 1000, 155, Color{80, 50, 25, 90});
    DrawCircleLines(1000, 1000, 158, Color{50, 32, 18, 50});

    // Hazard striping at hub mouth (decorative, not collision)
    const Vector2 junctions[] = {{1000, 820}, {1000, 1180}, {820, 1000}, {1180, 1000}};
    for (Vector2 p : junctions)
        DrawRectangle(p.x - 32, p.y - 6, 64, 12, Color{110, 82, 28, 120});
}

void Game::drawPlayerHud(const Player &player)
{
    const int pad = 12;
    const int panelW = 230;
    const int panelH = 96;
    const int x = pad;
    const int y = pad;
    const int inner = 10;
    const int barW = panelW - inner * 2;
    const int barH = 18;

    DrawRectangle(x, y, panelW, panelH, Color{0, 0, 0, 200});
    DrawRectangleLines(x, y, panelW, panelH, Color{170, 170, 170, 255});

    // Health
    const int healthBarY = y + inner + 12;
    DrawText("HEALTH", x + inner, y + inner, 12, LIGHTGRAY);
    DrawRectangle(x + inner, healthBarY, barW, barH, Color{90, 25, 25, 255});
    const int healthFill = static_cast<int>(
        (player.getHealth() / static_cast<float>(DEFAULT_PLAYER_HEALTH)) * barW);
    DrawRectangle(x + inner, healthBarY, healthFill, barH, GREEN);
    DrawText(TextFormat("%d / %d", player.getHealth(), DEFAULT_PLAYER_HEALTH),
             x + inner + 4, healthBarY + 2, 14, WHITE);

    // Sprint
    const int sprintBarY = healthBarY + barH + 18;
    DrawText("SPRINT  [F]", x + inner, sprintBarY - 14, 12, Color{220, 180, 80, 255});

    const float ratio = std::max(0.0f, std::min(1.0f, player.getSprintMeterRatio()));
    const int sprintFill = static_cast<int>(ratio * barW);

    Color sprintBg = {35, 35, 40, 255};
    Color sprintFillColor;
    const char *statusText;

    if (player.isSprintActive())
    {
        sprintFillColor = {255, 185, 40, 255};
        statusText = "BOOSTING";
    }
    else if (!player.isSprintReady())
    {
        sprintFillColor = {130, 130, 140, 255};
        statusText = "RECHARGING";
    }
    else
    {
        sprintFillColor = {255, 200, 60, 255};
        statusText = "READY";
    }

    DrawRectangle(x + inner, sprintBarY, barW, barH, sprintBg);
    if (sprintFill > 0)
        DrawRectangle(x + inner, sprintBarY, sprintFill, barH, sprintFillColor);
    DrawRectangleLines(x + inner, sprintBarY, barW, barH, Color{100, 100, 105, 255});

    int statusWidth = MeasureText(statusText, 12);
    DrawText(statusText, x + inner + barW - statusWidth, sprintBarY + 3, 12, sprintFillColor);
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

void Game::drawScore()
{
    const char *scoreText = TextFormat("Score: %.0f", score);
    const int fontSize = 20;
    const int padding = 10;

    int textWidth = MeasureText(scoreText, fontSize);
    int textHeight = fontSize;

    DrawRectangle(viewportW - textWidth - padding * 2, 0, textWidth + padding * 2, textHeight + padding * 2,
                  Color{0, 0, 0, 150});
    DrawText(scoreText, viewportW - textWidth - padding, padding, fontSize, WHITE);
}