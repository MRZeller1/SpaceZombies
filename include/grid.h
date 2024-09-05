#ifndef GRID_H
#define GRID_H

#include <raylib.h>
#include <vector>
#include "gridNode.h"

class Grid
{
private:
     int cellSize = 100;
     int window_size;
     int rows, cols;
     Vector2 origin;
     std::vector<std::vector<GridNode *>> grid;

public:
     Grid(int width, int height, int cellSize);

     void draw() const;
     void initGrid();
     Vector2 getGridPosition(float x, float y);
     Vector2 getObjectGridSize(float objectWidth, float objectHeight);
     Vector2 getRandomUnocupiedPosition();
     void setCellAttributes(int x, int y, int type);
     int getCellSize();
     GridNode *getGridNode(int x, int y);
     void updateGridNode(int x, int y, int attribute);
     void resetDistance();
     void addCollisionBox(Rectangle collisionBox);
     bool isValidCell(float x, float y);
};
#endif