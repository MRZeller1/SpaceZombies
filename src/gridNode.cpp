#include <gridNode.h>
#include <iostream>

GridNode::GridNode(Vector2 position, int cellSize)
{
    std::cout << "GridNode constructor called" << std::endl;
    // segfaults here
    // loadTextures();
    // this->current_texture = player;
    this->color = WHITE;
    // segfaults here
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

void GridNode::setAttributes(int type)
{
    // prevent zombie from taking players spot
    if (type == 2 && this->type == 1)
    {
        this->type = 1;
    }
    else
    {
        this->type = type;
    }
    if (type == 1)
    {
        // this->current_texture = player;
        this->color = GREEN;
        this->distance = 0;
    }
    else if (type == 2 && this->type != 1)
    {
        this->color = RED;
    }
    else if (type == 3)
    {
        // this->current_texture = object;
        this->color = RED;
        this->isObject = true;
    }
    else if (type == 0)
    {
        this->color = WHITE;
    }
}

void GridNode::draw() const
{
    // Draw texture from top left corner
    // DrawTexture(current_texture, position.x, position.y, WHITE);
    DrawRectangle(position.x, position.y, cellSize, cellSize, color);
    drawDistance();
    if (direction.x != 0 || direction.y != 0)
    {
        drawArrow();
        drawDistance();
    }
}
// We need to add null checks for each one of these but dont change the direction

void GridNode::loadTextures()
{
    /*const std::string resourcePath = "resources/";
    const std::vector<std::pair<std::string, Texture2D*>> textures = {
        {"up.png", &up_texture},
        {"down.png", &down_texture},
        {"left.png", &left_texture},
        {"right.png", &right_texture},
        {"player_cell.png", &player},
        {"object_cell.png", &object}
    };

    for (const auto& [fileName, texturePtr] : textures) {
        *texturePtr = LoadTexture((resourcePath + fileName).c_str());
        if (texturePtr->id == 0) {
            std::cerr << "Error loading texture: " << fileName << std::endl;
        }
    }*/
    // loads texture but then throws invalid memory access
    up_texture = LoadTexture("resources/player_left.png");
    // does not reach this point
    std::cout << "loaded Up" << std::endl;
    // down_texture = LoadTexture("resources/down.png");
    // left_texture = LoadTexture("resources/left.png");
    // right_texture = LoadTexture("resources/right.png");
    // player = LoadTexture("resources/player_cell.png");
    // object = LoadTexture("resources/object_cell.png");
    // if(up_texture.id == 0 || down_texture.id == 0 || left_texture.id == 0 || right_texture.id == 0 || up_left_texture.id == 0 || up_right_texture.id == 0 || down_left_texture.id == 0 || down_right_texture.id == 0 || player.id == 0 || object.id == 0) {
    //     std::cerr << "Error loading textures" << std::endl;
    //     exit(1);
    // }
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

// WORKING MEHTOD
void GridNode::update()
{
    int nextDistance = this->distance;
    this->setDirection();
    if (this->left != nullptr && !this->left->isObject && this->left->distance >= nextDistance + 1)
    {
        this->left->distance = nextDistance + 1;
        this->left->update();
    }
    if (this->right != nullptr && !this->right->isObject && this->right->distance >= nextDistance + 1)
    {
        this->right->distance = nextDistance + 1;
        this->right->update();
    }
    if (this->up != nullptr && !this->up->isObject && this->up->distance >= nextDistance + 1)
    {
        this->up->distance = nextDistance + 1;
        this->up->update();
    }
    if (this->down != nullptr && !this->down->isObject && this->down->distance >= nextDistance + 1)
    {
        this->down->distance = nextDistance + 1;
        this->down->update();
    }
}

void GridNode::setDirection()
{
    if (distance == 0)
    {
        direction = {0, 0};
        return;
    }
    int leftDistance = (left && !left->isObject) ? left->distance : 100;
    int rightDistance = (right && !right->isObject) ? right->distance : 100;
    int upDistance = (up && !up->isObject) ? up->distance : 100;
    int downDistance = (down && !down->isObject) ? down->distance : 100;

    int minDistance = std::min({leftDistance, rightDistance, upDistance, downDistance});

    float dx = 0, dy = 0;
    if (leftDistance == minDistance)
        dx -= 1;
    if (rightDistance == minDistance)
    {
        if (dx == 0)
            dx += 1;
    }
    if (upDistance == minDistance)
        dy -= 1;
    if (downDistance == minDistance)
    {
        if (dy == 0)
            dy += 1;
    }

    direction = {dx, dy};
}

void GridNode::drawArrow() const
{
    // Arrow dimensions and configuration
    const float length = 20.0f;     // Length of the arrow
    const float headLength = 10.0f; // Length of the arrow head (increased for more pronounced effect)
    const float headWidth = 4.0f;   // Width of the arrow head base

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
    DrawLineEx(center, endPoint, 2.0f, BLACK); // Adjust thickness and color as needed

    // Calculate arrow head points, adjusting to create a sharper, narrower arrow head
    Vector2 headLeft = {
        endPoint.x - headWidth * (-normalizedDirection.y),
        endPoint.y - headWidth * normalizedDirection.x};
    Vector2 headRight = {
        endPoint.x + headWidth * (-normalizedDirection.y),
        endPoint.y + headWidth * normalizedDirection.x};

    // Draw the arrow head: from the end point to the left and right points calculated
    DrawLineEx(endPoint, headLeft, 2.0f, BLACK);
    DrawLineEx(endPoint, headRight, 2.0f, BLACK);

    // Close the arrowhead by connecting left and right points (optional for filled arrowhead look)
    DrawLineEx(headLeft, headRight, 2.0f, BLACK);
}
void GridNode::drawDistance() const
{
    // Draw distance value centered in the node
    DrawText(std::to_string(distance).c_str(), position.x + cellSize / 2 - MeasureText(std::to_string(distance).c_str(), 20) / 2, position.y + cellSize / 2 - 20, 20, BLUE);
}
