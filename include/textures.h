#ifndef TEXTURES_H
#define TEXTURES_H

#include <raylib.h>

namespace GameTextures
{
    void Load();
    void Unload();

    Texture2D Bug();
    Texture2D Zombie();
    Texture2D ZombieWalk1();
    Texture2D ZombieWalk2();
}

#endif
