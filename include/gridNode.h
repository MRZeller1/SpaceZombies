#ifndef GRIDNODE_H
#define GRIDNODE_H

#include <raylib.h>
#include <vector>

class GridNode
{
private:
    Vector2 position;
    Color color;
    int cellSize;
    int type; // 0 = empty 1 = player, 2 = zombie, 3 = object
    bool isObject;
    Vector2 direction;
    int distance;
    const int minDistance = 10000;
    // neighbor nodes

    GridNode *up;
    GridNode *down;
    GridNode *left;
    GridNode *right;
    GridNode *upLeft;
    GridNode *upRight;
    GridNode *downLeft;
    GridNode *downRight;

public:
    GridNode(Vector2 position, int cellSize);
    GridNode *getGridNode(int x, int y);
    std::string toString();
    void setType(int type) { this->type = type; }
    void setAttributes(int type);
    void draw() const;
    void drawArrow() const;
    void drawDistance() const;
    void setLeft(GridNode *left) { this->left = left; }
    void setRight(GridNode *right) { this->right = right; }
    void setUp(GridNode *up) { this->up = up; }
    void setDown(GridNode *down) { this->down = down; }
    void resetDistance() {this->distance = 10000; } // Use the minDistance constant you've defined
    void setLeftAttribute(int type);
    void setRightAttribute(int type);
    void setUpAttribute(int type);
    void setDownAttribute(int type);
    void setDirection();
    Vector2 getDirection() { return direction; }
    int getType() { return type; }
    int getDistance() { return distance; }
    bool isObjectNode() { return isObject; }
    void setDirection(Vector2 direction) { this->direction = direction; }
    void setDistance(int distance) { this->distance = distance; }
    GridNode *getLeft() { return left; }
    GridNode *getRight() { return right; }
    GridNode *getUp() { return up; }
    GridNode *getDown() { return down; }
    GridNode *getUpLeft() { return upLeft; }
    GridNode *getUpRight() { return upRight; }
    GridNode *getDownLeft() { return downLeft; }
    GridNode *getDownRight() { return downRight; }
    Vector2 getPosition() { return position; }
};
#endif