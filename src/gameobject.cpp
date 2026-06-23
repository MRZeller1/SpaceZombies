#include "gameobject.h"
#include "grid.h"
#include "collisionMap.h"
#include <algorithm>
#include <cmath>

static Color defaultBorder(Color fill)
{
    return Color{
        (unsigned char)std::min(255, fill.r + 28),
        (unsigned char)std::min(255, fill.g + 28),
        (unsigned char)std::min(255, fill.b + 28),
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
    Color shadow = {0, 0, 0, 90};

    DrawRectangle(r.x + 3, r.y + 3, r.width, r.height, shadow);
    DrawRectangle(r.x, r.y, r.width, r.height, color);

    switch (obstacleType)
    {
    case ObstacleType::Hull:
    {
        DrawRectangleLinesEx(r, 2.0f, borderColor);
        DrawRectangle(r.x + 4, r.y + 4, r.width - 8, 3, Color{55, 200, 220, 90});
        for (float y = r.y + 14; y < r.y + r.height - 6; y += 12)
            DrawLine(r.x + 6, y, r.x + r.width - 6, y, Color{40, 48, 62, 80});
        break;
    }
    case ObstacleType::SupplyPod:
        DrawRectangleLinesEx(r, 2.0f, borderColor);
        DrawRectangle(r.x + r.width * 0.15f, r.y + r.height * 0.15f,
                      r.width * 0.7f, r.height * 0.7f, Color{20, 80, 110, 120});
        DrawCircleV({r.x + r.width * 0.5f, r.y + r.height * 0.35f}, r.width * 0.12f, Color{80, 220, 255, 200});
        break;
    case ObstacleType::Bulkhead:
        DrawRectangleLinesEx(r, 1.5f, borderColor);
        DrawRectangle(r.x + r.width - 5, r.y + 5, 2, r.height - 10, Color{80, 220, 255, 120});
        DrawRectangle(r.x + 5, r.y + 5, 2, r.height - 10, Color{80, 220, 255, 60});
        break;
    case ObstacleType::ReactorPillar:
        DrawRectangleLinesEx(r, 2.0f, borderColor);
        DrawCircleV({r.x + r.width * 0.5f, r.y + r.height * 0.5f}, std::min(r.width, r.height) * 0.28f,
                    Color{40, 170, 200, 220});
        DrawCircleV({r.x + r.width * 0.5f, r.y + r.height * 0.5f}, std::min(r.width, r.height) * 0.14f,
                    Color{160, 245, 255, 255});
        break;
    case ObstacleType::MoonRock:
        DrawCircleV({r.x + r.width * 0.5f, r.y + r.height * 0.5f}, r.width * 0.48f, color);
        DrawCircleV({r.x + r.width * 0.35f, r.y + r.height * 0.38f},
                    r.width * 0.14f, Color{72, 70, 68, 180});
        DrawCircleV({r.x + r.width * 0.62f, r.y + r.height * 0.58f},
                    r.width * 0.1f, Color{68, 66, 64, 160});
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
