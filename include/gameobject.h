#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "forward_declarations.h"
#include "raylib.h"
#include <iostream>

enum class ObstacleType
{
    Hull,
    SupplyPod,
    Bulkhead,
    ReactorPillar,
    MoonRock
};

class GameObject
{
private:
    float x, y;
    Texture2D texture;
    Rectangle *collisionRect;
    Color color;
    Color borderColor;
    ObstacleType obstacleType;
    Grid &grid;
    CollisionMap &collisionMap;

public:
    GameObject(float startx, float starty, const char *texturePath, float width, float height, float offsetX, float offsetY, Grid &grid);
    GameObject(float startx, float starty, Color color, float width, float height, float offsetX, float offsetY,
               Grid &grid, CollisionMap &collisionMap,
               Color borderColor = Color{0, 0, 0, 0}, ObstacleType type = ObstacleType::Bulkhead);
    ~GameObject();
    bool hasTexture();
    Rectangle getCollisionRect() const { return *collisionRect; }
    void draw();
    void drawColor();
    void setGridNodeAttributes(Vector2 gridPos, Vector2 gridSize);
};
#endif
