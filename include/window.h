#ifndef WINDOW_H
#define WINDOW_H

#include <raylib.h>
#include "player.h"
#include "zombie.h"
#include "bug.h"
#include "GameObject.h"
#include <vector>
#include "grid.h"
#include <iostream>
class Window
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
    std::vector<GameObject *> objects;

public:
    Window(int width, int height, const Grid &grid, CollisionMap &collisionMap, const char *title);
    ~Window();
    void addObject(GameObject *object);
    std::vector<GameObject *> getObjects();
    void run(Player &player, std::vector<Zombie *> &zombies, std::vector<Bug *> &bugs);
    void stop();
    void createZombies(std::vector<Zombie *> &zombies);
    void createBugs(std::vector<Bug *> &bugs);
};

#endif