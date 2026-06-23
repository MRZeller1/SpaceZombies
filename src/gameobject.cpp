#include "gameobject.h"
#include "grid.h"
#include "collisionMap.h"
#include <algorithm>
#include <cmath>

static Color defaultBorder(Color fill)
{
    return Color{
        (unsigned char)std::min(255, fill.r + 24),
        (unsigned char)std::min(255, fill.g + 24),
        (unsigned char)std::min(255, fill.b + 24),
        255};
}

GameObject::GameObject(float startx, float starty, Color color, float width, float height, float offsetX, float offsetY,
                       Grid &grid, CollisionMap &collisionMap, Color borderColor, ObstacleType type)
    : x(startx), y(starty), color(color), borderColor(borderColor.a == 0 ? defaultBorder(color) : borderColor),
      obstacleType(type), grid(grid), collisionMap(collisionMap), texture{0}
{
    collisionRect = new Rectangle{x + offsetX, y + offsetY, width, height};
    Vector2 gridPos = grid.getGridPosition(x, y);
    Vector2 gridSize = grid.getObjectGridSize(width, height);

    collisionMap.addStaticCollisionRectangle(collisionRect);
    setGridNodeAttributes(gridPos, gridSize);
}

bool GameObject::hasTexture()
{
    return texture.id != 0;
}

GameObject::~GameObject()
{
    if (texture.id != 0)
        UnloadTexture(texture);
}

void GameObject::draw()
{
    Rectangle r = *collisionRect;
    Color shadow = {0, 0, 0, 110};

    DrawRectangle(r.x + 3, r.y + 3, r.width, r.height, shadow);
    DrawRectangle(r.x, r.y, r.width, r.height, color);

    switch (obstacleType)
    {
    case ObstacleType::Hull:
        DrawRectangleLinesEx(r, 2.0f, borderColor);
        DrawRectangle(r.x + 4, r.y + 4, r.width - 8, 3, Color{55, 48, 42, 120});
        for (float y = r.y + 14; y < r.y + r.height - 6; y += 12)
            DrawLine(r.x + 6, y, r.x + r.width - 6, y, Color{28, 26, 30, 90});
        break;
    case ObstacleType::SupplyPod:
        DrawRectangleLinesEx(r, 2.0f, borderColor);
        DrawRectangle(r.x + r.width * 0.12f, r.y + r.height * 0.12f,
                      r.width * 0.76f, r.height * 0.76f, Color{32, 28, 24, 180});
        DrawRectangle(r.x + r.width * 0.38f, r.y + r.height * 0.28f,
                      r.width * 0.24f, r.height * 0.12f, Color{140, 110, 50, 200});
        break;
    case ObstacleType::Bulkhead:
        DrawRectangleLinesEx(r, 1.5f, borderColor);
        DrawRectangle(r.x + r.width - 5, r.y + 5, 2, r.height - 10, Color{90, 82, 72, 80});
        DrawRectangle(r.x + 5, r.y + 5, 2, r.height - 10, Color{70, 64, 58, 50});
        for (float x = r.x + 10; x < r.x + r.width - 10; x += 14)
            DrawRectangle(x, r.y + r.height * 0.45f, 7, 4, Color{130, 95, 35, 160});
        break;
    case ObstacleType::ReactorPillar:
        DrawRectangleLinesEx(r, 2.0f, borderColor);
        DrawCircleV({r.x + r.width * 0.5f, r.y + r.height * 0.5f}, std::min(r.width, r.height) * 0.28f,
                    Color{120, 55, 20, 200});
        DrawCircleV({r.x + r.width * 0.5f, r.y + r.height * 0.5f}, std::min(r.width, r.height) * 0.14f,
                    Color{220, 130, 45, 255});
        break;
    case ObstacleType::CargoCrate:
        DrawRectangleLinesEx(r, 2.0f, borderColor);
        DrawLineEx({r.x + 5, r.y + 5}, {r.x + r.width - 5, r.y + r.height - 5}, 2.0f, Color{30, 26, 22, 160});
        DrawLineEx({r.x + r.width - 5, r.y + 5}, {r.x + 5, r.y + r.height - 5}, 2.0f, Color{30, 26, 22, 160});
        DrawRectangle(r.x + r.width * 0.35f, r.y + 4, r.width * 0.3f, 5, Color{110, 85, 40, 180});
        break;
    }
}

void GameObject::setGridNodeAttributes(Vector2 gridPos, Vector2 gridSize)
{
    for (int i = 0; i < gridSize.x; i++)
    {
        for (int j = 0; j < gridSize.y; j++)
        {
            int cellX = gridPos.x + i;
            int cellY = gridPos.y + j;
            if (grid.isValidCell(cellX, cellY))
            {
                GridNode *node = grid.getGridNode(cellX, cellY);
                if (node)
                    node->setAttributes(3);
            }
        }
    }
}
