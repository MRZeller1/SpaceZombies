#include <gridNode.h>
#include <iostream>

GridNode::GridNode(Vector2 position, int cellSize)
{
    std::cout << "GridNode constructor called" << std::endl;
    this->color = WHITE;
    this->position = position;
    this->cellSize = cellSize;
    this->type = 0;
    this->isObject = false;
    this->direction = {0, 0};
    this->distance = 100;
}

std::string GridNode::toString()
{
    return "GridNode: " + std::to_string(position.x) + ", " + std::to_string(position.y);
}

GridNode *GridNode::getGridNode(int x, int y)
{
    if (position.x == x && position.y == y)
    {
        return this;
    }
    return nullptr;
}
void GridNode::setDirection() {
        // Player base case
        if (distance == 0) {
            color = GREEN;
            direction = {0, 0};
            return;
        }

        int minDistance = this->minDistance;
        Vector2 tempDirection = {0, 0};

        std::vector<std::pair<GridNode*, Vector2>> neighbors = {
            {left, {-1, 0}}, {right, {1, 0}}, {up, {0, -1}}, {down, {0, 1}},
            {upLeft, {-1, -1}}, {upRight, {1, -1}}, {downLeft, {-1, 1}}, {downRight, {1, 1}}
        };

        // First pass: find the minimum distance
        for (const auto& [neighbor, dir] : neighbors) {
            if (neighbor && !neighbor->isObject && neighbor->distance < minDistance) {
                minDistance = neighbor->distance;
            }
        }

        // Second pass: accumulate directions of all minimum distance neighbors
        for (const auto& [neighbor, dir] : neighbors) {
            if (neighbor && !neighbor->isObject && neighbor->distance == minDistance) {
                tempDirection.x += dir.x;
                tempDirection.y += dir.y;
            }
        }

        // Normalize the direction
        float length = std::sqrt(tempDirection.x * tempDirection.x + tempDirection.y * tempDirection.y);
        if (length > 0) {
            direction.x = tempDirection.x / length;
            direction.y = tempDirection.y / length;
        } else {
            direction = {0, 0};
        }
    }

void GridNode::setAttributes(int type)
{
    // prevent zombie from taking players spot
    switch (type)
    {
        case 1: // Player
            this->type = 1;
            this->color = GREEN;
            this->distance = 0;
            break;

        case 2: // Zombie
            if (this->type != 1) // Prevent zombie from taking player's spot
            {
                this->type = 2;
                this->color = RED;
            }
            break;

        case 3: // Object
            this->type = 3;
            this->isObject = true;
            break;

        case 0: // Empty space
            this->type = 0;
            this->color = WHITE;
            break;

        default: 
            this->type = type;
            break;
    }
}

void GridNode::draw() const
{
    DrawRectangle(position.x, position.y, cellSize, cellSize, color);
    drawDistance();
    if (direction.x != 0 || direction.y != 0)
    {
        drawArrow();
        drawDistance();
    }
}
void GridNode::setLeftAttribute(int type)
{
    if (left != nullptr)
    {
        left->setAttributes(type);
    }
}
void GridNode::setRightAttribute(int type)
{
    if (right != nullptr)
    {
        right->setAttributes(type);
    }
}
void GridNode::setUpAttribute(int type)
{
    if (up != nullptr)
    {
        up->setAttributes(type);
    }
}
void GridNode::setDownAttribute(int type)
{
    if (down != nullptr)
    {
        down->setAttributes(type);
    }
}

void GridNode::drawArrow() const
{
    // Arrow dimensions and configuration
    const float length = 10.0f;     // Length of the arrow
    const float headLength = 50.0f; // Length of the arrow head (increased for more pronounced effect)
    const float headWidth = 2.0f;   // Width of the arrow head base

    // Center of the node
    Vector2 center = {position.x + cellSize / 2, position.y + cellSize / 2};

    // Normalize direction for consistent arrow length
    float directionMagnitude = sqrt(direction.x * direction.x + direction.y * direction.y);
    // Ensure division by zero does not occur
    if (directionMagnitude == 0)
        directionMagnitude = 1;
    Vector2 normalizedDirection = {direction.x / directionMagnitude, direction.y / directionMagnitude};

    // Calculate end point of the arrow based on direction
    Vector2 endPoint = {
        center.x + normalizedDirection.x * length,
        center.y + normalizedDirection.y * length};

    // Draw the main line of the arrow
    DrawLineEx(center, endPoint, 1.0f, BLACK); // Adjust thickness and color as needed

    // Calculate arrow head points, adjusting to create a sharper, narrower arrow head
    Vector2 headLeft = {
        endPoint.x - headWidth * (-normalizedDirection.y),
        endPoint.y - headWidth * normalizedDirection.x};
    Vector2 headRight = {
        endPoint.x + headWidth * (-normalizedDirection.y),
        endPoint.y + headWidth * normalizedDirection.x};

    // Draw the arrow head: from the end point to the left and right points calculated
    DrawLineEx(endPoint, headLeft, 1.0f, BLACK);
    DrawLineEx(endPoint, headRight, 1.0f, BLACK);

    // Close the arrowhead by connecting left and right points (optional for filled arrowhead look)
    DrawLineEx(headLeft, headRight, 1.0f, BLACK);
}
void GridNode::drawDistance() const
{
    // Draw distance value centered in the node
    DrawText(std::to_string(distance).c_str(), position.x + cellSize / 2 - MeasureText(std::to_string(distance).c_str(), 10) / 2, position.y + cellSize / 2 - 6, 10, BLUE);
}
 