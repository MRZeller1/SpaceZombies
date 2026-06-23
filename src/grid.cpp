#include "grid.h"
#include "collisionMap.h"

namespace
{
Rectangle makeSpawnRect(float centerX, float centerY, float width, float height)
{
    return {centerX - width * 0.5f, centerY - height * 0.5f, width, height};
}

bool isFarEnough(float x, float y, float avoidX, float avoidY, float minDistance)
{
    if (minDistance <= 0.0f)
        return true;
    float dx = x - avoidX;
    float dy = y - avoidY;
    return dx * dx + dy * dy >= minDistance * minDistance;
}
}

// constructor for the grid         
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
// initialize the grid
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
            if (i > 0 && j > 0)
            {
                grid[i][j]->setUpLeft(grid[i - 1][j - 1]);
                grid[i - 1][j - 1]->setDownRight(grid[i][j]);
            }
            if (i > 0 && j < cols - 1)
            {
                grid[i][j]->setUpRight(grid[i - 1][j + 1]);
                grid[i - 1][j + 1]->setDownLeft(grid[i][j]);
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
// get a random unoccupied position
Vector2 Grid::getRandomUnocupiedPosition()
{
    int x = GetRandomValue(0, cols - 1);
    int y = GetRandomValue(0, rows - 1);
    while (grid[y][x]->getType() != 0 && grid[y][x]->getDistance() < 5)
    {
        x = GetRandomValue(0, cols - 1);
        y = GetRandomValue(0, rows - 1);
    }
    return {static_cast<float>(x) * cellSize + cellSize * 0.5f, static_cast<float>(y) * cellSize + cellSize * 0.5f};
}

Vector2 Grid::getSpawnPositionAwayFrom(float centerX, float centerY, float minDistance)
{
    const int margin = 4;
    const float minDistSq = minDistance * minDistance;

    for (int attempt = 0; attempt < 120; attempt++)
    {
        int x = GetRandomValue(margin, cols - 1 - margin);
        int y = GetRandomValue(margin, rows - 1 - margin);
        if (grid[y][x]->isObjectNode())
            continue;

        float worldX = x * cellSize + cellSize * 0.5f;
        float worldY = y * cellSize + cellSize * 0.5f;
        float dx = worldX - centerX;
        float dy = worldY - centerY;
        if (dx * dx + dy * dy >= minDistSq)
            return {worldX, worldY};
    }

    bool right = centerX < (cols * cellSize) * 0.5f;
    bool down = centerY < (rows * cellSize) * 0.5f;
    int x = right ? cols - margin - 1 : margin;
    int y = down ? rows - margin - 1 : margin;
    return {x * cellSize + cellSize * 0.5f, y * cellSize + cellSize * 0.5f};
}

bool Grid::canReach(float fromX, float fromY, float toX, float toY)
{
    Vector2 from = getGridPosition(fromX, fromY);
    Vector2 to = getGridPosition(toX, toY);
    int sx = static_cast<int>(from.x);
    int sy = static_cast<int>(from.y);
    int tx = static_cast<int>(to.x);
    int ty = static_cast<int>(to.y);

    if (!isValidCell(sx, sy) || !isValidCell(tx, ty))
        return false;
    if (grid[sy][sx]->isObjectNode() || grid[ty][tx]->isObjectNode())
        return false;

    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    std::queue<std::pair<int, int>> cells;
    cells.push({sx, sy});
    visited[sy][sx] = true;

    const int dx[] = {-1, 1, 0, 0};
    const int dy[] = {0, 0, -1, 1};

    while (!cells.empty())
    {
        auto [cx, cy] = cells.front();
        cells.pop();
        if (cx == tx && cy == ty)
            return true;

        for (int i = 0; i < 4; i++)
        {
            int nx = cx + dx[i];
            int ny = cy + dy[i];
            if (!isValidCell(nx, ny) || visited[ny][nx])
                continue;
            if (grid[ny][nx]->isObjectNode())
                continue;
            visited[ny][nx] = true;
            cells.push({nx, ny});
        }
    }
    return false;
}

Vector2 Grid::getClearSpawnPosition(CollisionMap &collisionMap, float width, float height,
                                    float preferredX, float preferredY,
                                    float avoidX, float avoidY, float minDistance)
{
    const int margin = 2;

    auto isBlocked = [&](float centerX, float centerY) {
        return collisionMap.isSpawnBlocked(makeSpawnRect(centerX, centerY, width, height));
    };

    auto cellCenter = [&](int gx, int gy) {
        return Vector2{gx * cellSize + cellSize * 0.5f, gy * cellSize + cellSize * 0.5f};
    };

    auto isValidSpawn = [&](float centerX, float centerY, bool checkDistance) {
        if (isBlocked(centerX, centerY))
            return false;
        if (checkDistance && !isFarEnough(centerX, centerY, avoidX, avoidY, minDistance))
            return false;
        if (minDistance > 0.0f && !canReach(centerX, centerY, avoidX, avoidY))
            return false;
        return true;
    };

    if (isValidSpawn(preferredX, preferredY, minDistance > 0.0f))
        return {preferredX, preferredY};

    for (int attempt = 0; attempt < 150; attempt++)
    {
        int gx = GetRandomValue(margin, cols - 1 - margin);
        int gy = GetRandomValue(margin, rows - 1 - margin);
        if (grid[gy][gx]->isObjectNode())
            continue;

        Vector2 candidate = cellCenter(gx, gy);
        if (isValidSpawn(candidate.x, candidate.y, true))
            return candidate;
    }

    Vector2 best = cellCenter(margin, margin);
    float bestScore = -1.0f;

    for (bool requireDistance : {true, false})
    {
        for (int gy = margin; gy < rows - margin; gy++)
        {
            for (int gx = margin; gx < cols - margin; gx++)
            {
                if (grid[gy][gx]->isObjectNode())
                    continue;

                Vector2 candidate = cellCenter(gx, gy);
                if (!isValidSpawn(candidate.x, candidate.y, requireDistance))
                    continue;

                float dx = candidate.x - preferredX;
                float dy = candidate.y - preferredY;
                float score = dx * dx + dy * dy;
                if (score > bestScore)
                {
                    bestScore = score;
                    best = candidate;
                }
            }
        }
        if (bestScore >= 0.0f)
            break;
    }

    return best;
}

Vector2 Grid::getClearGroupSpawnPosition(CollisionMap &collisionMap, float unitWidth, float unitHeight,
                                         int gridSize, int spacing,
                                         float avoidX, float avoidY, float minDistance)
{
    const int margin = 2;

    auto groupBlocked = [&](float startX, float startY) {
        for (int i = 0; i < gridSize; i++)
        {
            for (int j = 0; j < gridSize; j++)
            {
                float cx = startX + i * spacing;
                float cy = startY + j * spacing;
                if (collisionMap.isSpawnBlocked(makeSpawnRect(cx, cy, unitWidth, unitHeight)))
                    return true;
            }
        }
        return false;
    };

    auto groupCenter = [&](float startX, float startY) {
        float span = (gridSize - 1) * spacing;
        return Vector2{startX + span * 0.5f, startY + span * 0.5f};
    };

    auto isValidGroup = [&](float startX, float startY, bool checkDistance) {
        if (groupBlocked(startX, startY))
            return false;
        Vector2 center = groupCenter(startX, startY);
        if (checkDistance && !isFarEnough(center.x, center.y, avoidX, avoidY, minDistance))
            return false;
        if (minDistance > 0.0f && !canReach(center.x, center.y, avoidX, avoidY))
            return false;
        return true;
    };

    for (int attempt = 0; attempt < 150; attempt++)
    {
        int gx = GetRandomValue(margin, cols - 1 - margin);
        int gy = GetRandomValue(margin, rows - 1 - margin);
        if (grid[gy][gx]->isObjectNode())
            continue;

        float startX = gx * cellSize + cellSize * 0.5f;
        float startY = gy * cellSize + cellSize * 0.5f;
        if (isValidGroup(startX, startY, true))
            return {startX, startY};
    }

    Vector2 best = {margin * cellSize + cellSize * 0.5f, margin * cellSize + cellSize * 0.5f};
    float bestScore = -1.0f;

    for (bool requireDistance : {true, false})
    {
        for (int gy = margin; gy < rows - margin; gy++)
        {
            for (int gx = margin; gx < cols - margin; gx++)
            {
                if (grid[gy][gx]->isObjectNode())
                    continue;

                float startX = gx * cellSize + cellSize * 0.5f;
                float startY = gy * cellSize + cellSize * 0.5f;
                if (!isValidGroup(startX, startY, requireDistance))
                    continue;

                Vector2 center = groupCenter(startX, startY);
                float dx = center.x - avoidX;
                float dy = center.y - avoidY;
                float score = dx * dx + dy * dy;
                if (score > bestScore)
                {
                    bestScore = score;
                    best = {startX, startY};
                }
            }
        }
        if (bestScore >= 0.0f)
            break;
    }

    return best;
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
    if (!isValidCell(x, y))
        return;
    grid[y][x]->setAttributes(type);
}

int Grid::getCellSize()
{
    return this->cellSize;
}

GridNode *Grid::getGridNode(int x, int y)
{
    if (!isValidCell(x, y))
        return nullptr;
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
    grid[y][x]->setAttributes(attribute);
}

bool Grid::isValidCell(float x, float y)
{
    int gridX = static_cast<int>(x);
    int gridY = static_cast<int>(y);

    if (gridX < 0 || gridY < 0)
        return false;
    if (gridX > grid[0].size() - 1)
        return false;
    if (gridY > grid.size() - 1)
        return false;
    return true;
}

void Grid::updateDistances(Vector2 playerPos)
{
    std::queue<GridNode*> cellsToCheck;
    Vector2 gridPos = getGridPosition(playerPos.x, playerPos.y);
    GridNode* playerNode = grid[gridPos.y][gridPos.x];
    
    // Reset distances
    for (auto& row : grid) {
        for (auto& cell : row) {
            cell->setDistance(std::numeric_limits<int>::max());
        }
    }

    playerNode->setDistance(0);
    cellsToCheck.push(playerNode);

    while (!cellsToCheck.empty()) {
        GridNode* currentNode = cellsToCheck.front();
        cellsToCheck.pop();

        std::vector<GridNode*> neighbors = {currentNode->getLeft(), currentNode->getRight(), currentNode->getUp(), currentNode->getDown()};
        for (GridNode* neighbor : neighbors) {
            if (neighbor && !neighbor->isObjectNode() && neighbor->getDistance() > currentNode->getDistance() + 1) {
                neighbor->setDistance(currentNode->getDistance() + 1);
                cellsToCheck.push(neighbor);
            }
        }
    }
}

void Grid::partialUpdate(Vector2 playerPos, int radius) {
        Vector2 gridPos = getGridPosition(playerPos.x, playerPos.y);
        if (!isValidCell(gridPos.x, gridPos.y))
            return;

        std::queue<GridNode*> cellsToCheck;
        GridNode* playerNode = grid[(int)gridPos.y][(int)gridPos.x];
        
        // Reset distances for all cells within the radius
        for (int y = std::max(0, (int)gridPos.y - radius); y < std::min(rows, (int)gridPos.y + radius + 1); ++y) {
            for (int x = std::max(0, (int)gridPos.x - radius); x < std::min(cols, (int)gridPos.x + radius + 1); ++x) {
                grid[y][x]->resetDistance();
            }
        }

        playerNode->setDistance(0);
        cellsToCheck.push(playerNode);

        while (!cellsToCheck.empty()) {
            GridNode* currentNode = cellsToCheck.front();
            cellsToCheck.pop();

            if (currentNode->getDistance() > radius) continue;

            std::vector<GridNode*> neighbors = {
                currentNode->getLeft(), currentNode->getRight(), currentNode->getUp(), currentNode->getDown(),
                currentNode->getUpLeft(), currentNode->getUpRight(), currentNode->getDownLeft(), currentNode->getDownRight()
            };

            for (GridNode* neighbor : neighbors) {
                if (neighbor && !neighbor->isObjectNode() && neighbor->getDistance() > currentNode->getDistance() + 1) {
                    neighbor->setDistance(currentNode->getDistance() + 1);
                    cellsToCheck.push(neighbor);
                }
            }
        }

        // Update directions for affected cells
        for (int y = std::max(0, (int)gridPos.y - radius); y < std::min(rows, (int)gridPos.y + radius + 1); ++y) {
            for (int x = std::max(0, (int)gridPos.x - radius); x < std::min(cols, (int)gridPos.x + radius + 1); ++x) {
                grid[y][x]->setDirection();
            }
        }
    }