#include "grid.h"


// constructor
Grid::Grid(int width, int height, int cellSize)
{
    // Number of rows and columns dynamically calculated based on the window size and cell siz
    rows = height / cellSize;
    cols = width / cellSize;
    this->cellSize = cellSize;
    grid.resize(rows, std::vector<GridNode *>(cols, nullptr));
    initGrid();
    origin = {0, 0};

}

// drawing grid for game testing purposes
void Grid::draw() const
{
    for (int i = 0; i <= rows; ++i)
    {
        DrawLine(origin.x, origin.y + i * cellSize, origin.x + cols * cellSize, origin.y + i * cellSize, LIGHTGRAY);
    }
    for (int i = 0; i <= cols; ++i)
    {
        DrawLine(origin.x + i * cellSize, origin.y, origin.x + i * cellSize, origin.y + rows * cellSize, LIGHTGRAY);
    }
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            // std::cout << "Drawing grid node" << std::endl;
            if (grid[i][j] != nullptr)
            {
                grid[i][j]->draw();
            }
        }
    }
}
void Grid::initGrid()
{
    // Step 1: Create all nodes
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            Vector2 position = {static_cast<float>(j * cellSize), static_cast<float>(i * cellSize)};
            grid[i][j] = new GridNode(position, cellSize);
        }
    }

    // Step 2: Set neighbors for each node
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (j > 0)
            {
                grid[i][j]->setLeft(grid[i][j - 1]);
                grid[i][j - 1]->setRight(grid[i][j]);
            }
            if (i > 0)
            {
                grid[i][j]->setUp(grid[i - 1][j]);
                grid[i - 1][j]->setDown(grid[i][j]);
            }
        }
    }
}
// get the grid position of the object
Vector2 Grid::getGridPosition(float x, float y)
{
    float gridX = floor((x / this->cellSize)); // floor is used to get the integer value of the float
    float gridY = floor((y / this->cellSize));
    return {gridX, gridY};
}
Vector2 Grid::getRandomUnocupiedPosition()
{
    int x = GetRandomValue(0, cols - 1);
    int y = GetRandomValue(0, rows - 1);
    while (grid[y][x]->getType() != 0 && grid[y][x]->getDistance() < 5)
    {
        x = GetRandomValue(0, cols - 1);
        y = GetRandomValue(0, rows - 1);
    }
    return {static_cast<float>(x) * 50 + 25, static_cast<float>(y) * 50 + 25};
}
// get the size of object in cell units
Vector2 Grid::getObjectGridSize(float objectWidth, float objectHeight)
{
    // objects are made in bottom left corner, so we need to align them to the grid
    int gridX = ceil(objectWidth / this->cellSize);
    int gridY = ceil(objectHeight / this->cellSize);
    return {(float)gridX, (float)gridY};
}

// set the attributes of the cell
void Grid::setCellAttributes(int x, int y, int type)
{
    grid[y][x]->setAttributes(type);
}

int Grid::getCellSize()
{
    return this->cellSize;
}
GridNode *Grid::getGridNode(int x, int y)
{
    return grid[y][x];
}

void Grid::resetDistance()
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            grid[i][j]->setDistance(100);
        }
    }
}

void Grid::updateGridNode(int x, int y, int attribute)
{
    // grid[y][x]->update({0, 0});
    grid[y][x]->setAttributes(attribute);
}

bool Grid::isValidCell(float x, float y)
{
    int gridX = static_cast<int>(x);
    int gridY = static_cast<int>(y);

    // Check if the indices are within the bounds of the grid
    if (gridX > grid[0].size() - 1)
        return false;
    if (gridY > grid.size() - 1)
        return false;
    return true;
}