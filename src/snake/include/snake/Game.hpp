#pragma once

#include <cstddef>
#include <vector>

enum class Direction {
    Up,
    Down,
    Left,
    Right
};

struct Position {
    int x;
    int y;
};

class Game
{
private:
    int width;
    int height;
    
    int playerPoints;
    Position pointPosition;

    Position headPosition;

    Direction movementDirection;

    bool isGameOver;
    std::vector<Position> snakeBody;
public:
    Game(int width = 50, int height = 20);

    void run();

private:
    void update();
    void render() const;
    void moveSnake();
    bool checkCollision();
    void handleInput();
    char readInput();
    bool isNewDirOppositeDirection(Direction newDir, Direction currDir) const;
    void generateRandomPosition();
    bool isSnakeBodyPosition(int x, int y) const;
    char getCellSymbol(int x, int y) const;
};

