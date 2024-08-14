#include "gameobject.h"
#include <iostream>


GameObject::GameObject(float startx, float starty, Color color, float width, float height, float offsetX, float offsetY, Grid &grid, CollisionMap &collisionMap) : x(startx), y(starty), color(color), grid(grid), collisionMap(collisionMap)
{
    Vector2 alignedPos = grid.getGridPosition(x, y);
    Vector2 alignedSize = grid.getObjectGridSize(width, height);
    collisionRect = new Rectangle{x + offsetX, y + offsetY, width, height};
    // set cell attributes for each block in object, note that the objects are made from top left to bottom right cells are top left to bottom right
    Vector2 gridPos = grid.getGridPosition(x, y);
    Vector2 gridSize = grid.getObjectGridSize(width, height);

    collisionMap.addStaticCollisionRectangle(collisionRect);
    
    setGridNodeAttributes(gridPos, gridSize);
}
// Constructor where the user can specify collison zone size
bool GameObject::hasTexture()
{
    return texture.id != 0;
}

GameObject::~GameObject()
{
    UnloadTexture(texture);
}

void GameObject::draw()
{
    DrawRectangle(x, y, collisionRect->width, collisionRect->height, color);
    DrawRectangleLines(collisionRect->x, collisionRect->y, collisionRect->width, collisionRect->height, RED);
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
                {
                    node->setAttributes(3); // Assuming 3 represents an object
                }
            }
        }
    }
}
