#include "textures.h"

namespace
{
    Texture2D bugTexture = {0};
    Texture2D zombieTexture = {0};
    Texture2D zombieWalk1Texture = {0};
    Texture2D zombieWalk2Texture = {0};
    bool loaded = false;
}

void GameTextures::Load()
{
    if (loaded)
        return;

    bugTexture = LoadTexture("resources/bug.png");
    zombieTexture = LoadTexture("resources/zombie.png");
    zombieWalk1Texture = LoadTexture("resources/zombie_walk1.png");
    zombieWalk2Texture = LoadTexture("resources/zombie_walk2.png");
    loaded = true;
}

void GameTextures::Unload()
{
    if (!loaded)
        return;

    UnloadTexture(bugTexture);
    UnloadTexture(zombieTexture);
    UnloadTexture(zombieWalk1Texture);
    UnloadTexture(zombieWalk2Texture);

    bugTexture = {0};
    zombieTexture = {0};
    zombieWalk1Texture = {0};
    zombieWalk2Texture = {0};
    loaded = false;
}

Texture2D GameTextures::Bug() { return bugTexture; }
Texture2D GameTextures::Zombie() { return zombieTexture; }
Texture2D GameTextures::ZombieWalk1() { return zombieWalk1Texture; }
Texture2D GameTextures::ZombieWalk2() { return zombieWalk2Texture; }
