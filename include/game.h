#ifndef GAME_H
#define GAME_H

#include "grid.h"
#include "collisionMap.h"
#include "player.h"
#include <vector>


#include <iostream>
class Game
{
private:
    int m_width;
    int m_height;
    Grid grid;
    CollisionMap collisionMap;
    bool gameStart = false;
    const char *m_title;
    int zombieCount = 1;
    int aliveZombieCount = 0;
    bool gameOver;
    float score;
    float finalScore;
    float lastScoreUpdate;
    std::vector<GameObject *> objects;

public:
    Game(int width, int height, const Grid &grid, CollisionMap &collisionMap, const char *title);
    ~Game();
    void addObject(GameObject *object);
    std::vector<GameObject *> getObjects();
    void run(Player &player, std::vector<Zombie *> &zombies, std::vector<Bug *> &bugs, std::vector<Bullet *> &bullets);
    void createZombies(std::vector<Zombie *> &zombies);
    void createBugs(std::vector<Bug *> &bugs);
    void updateBullets(std::vector<Bullet*> &bullets, float deltaTime);
    void updateZombies(std::vector<Zombie *> &zombies, float deltaTime);
    void updateBugs(std::vector<Bug *> &bugs, float deltaTime);
    void drawBullets(Player &player);
    void drawHealthBar(const Player &player);
    void drawGameOverBanner(Player &player);
    void updateScore(float deltaTime);
    float getScore() const { return score; }
    void drawScore();
    

};

#endif