#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "forward_declarations.h"
#include "raylib.h"
#include <iostream>


class GameObject
{
private:
    float x, y;
    Texture2D texture;
    Rectangle *collisionRect;
    Color color;
    Grid &grid;
    CollisionMap &collisionMap;

public:
    GameObject(float startx, float starty, const char *texturePath, float width, float height, float offsetX, float offsetY, Grid &grid);
    GameObject(float startx, float starty, Color color, float width, float height, float offsetX, float offsetY, Grid &grid, CollisionMap &collisionMap);
    ~GameObject();
    bool hasTexture();
    Rectangle getCollisionRect() const { return *collisionRect; }
    void draw();
    void drawColor();
    void setGridNodeAttributes(Vector2 gridPos, Vector2 gridSize);
};
#endif
