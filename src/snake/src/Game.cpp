#include "snake/Game.hpp"
#include <cstdlib>
#include <iostream>
#include <chrono>
#include <thread>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <random>
#include <algorithm>

using namespace std::chrono_literals;

Game::Game(int width, int height)
    : width(width), height(height),
      playerPoints(0),
      headPosition{25, 10},
      movementDirection(Direction::Up),
      pendingDirection(Direction::Up),
      isGameOver(false)
{
    snakeBody.push_back({24, 10});
    snakeBody.push_back({23, 10});
}

void Game::run()
{
    generateRandomPosition();
    int delayMs = 400;
    while (!isGameOver)
    {
        delayMs = std::max(50, 400 - playerPoints * 5);
        //handleInput();
        update();
        render();
        if (movementDirection == Direction::Down ||
            movementDirection == Direction::Up)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
        }
    }

    if (isGameOver)
    {
        system("clear");
        std::cout << "Your points: " << playerPoints << "\n";
    }
}

void Game::render() const
{
    system("clear");

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            std::cout << getCellSymbol(x, y);
        }
        std::cout << "\n";
    }
}

char Game::getCellSymbol(int x, int y) const
{
    if (x == 0 || x == width - 1 || y == 0 || y == height - 1)
    {
        return '#';
    }

    if (x == headPosition.x && y == headPosition.y)
    {
        return '0';
    }

    if (isSnakeBodyPosition(x, y))
    {
        return 'o';
    }

    if (x == pointPosition.x && y == pointPosition.y)
    {
        return 'X';
    }

    return ' ';
}

void Game::update()
{
    moveSnake();
    checkCollision();
}

bool Game::isSnakeBodyPosition(int x, int y) const
{
    for (auto &segment : snakeBody)
    {
        if (segment.x == x && segment.y == y)
        {
            return true;
        }
    }
    return false;
}

void Game::moveSnake()
{
    movementDirection = pendingDirection;

    Position oldHeadPos = headPosition;

    switch (movementDirection)
    {
    case Direction::Up:
        headPosition.y--;
        break;
    case Direction::Down:
        headPosition.y++;
        break;
    case Direction::Left:
        headPosition.x--;
        break;
    case Direction::Right:
        headPosition.x++;
        break;
    default:
        break;
    };

    snakeBody.insert(snakeBody.begin(), oldHeadPos);
    if (!checkCollision())
    {
        snakeBody.pop_back();
    }
}

bool Game::isNewDirOppositeDirection(Direction newDir, Direction currDir) const
{
    return (currDir == Direction::Up && newDir == Direction::Down) ||
           (currDir == Direction::Down && newDir == Direction::Up) ||
           (currDir == Direction::Left && newDir == Direction::Right) ||
           (currDir == Direction::Right && newDir == Direction::Left);
}

bool Game::checkCollision()
{
    if (headPosition.x <= 0 || headPosition.x >= (width - 1) ||
        headPosition.y <= 0 || headPosition.y >= (height - 1))
    {
        isGameOver = true;
        return true;
    }

    if (isSnakeBodyPosition(headPosition.x, headPosition.y))
    {
        isGameOver = true;
        return true;
    }

    if (headPosition.x == pointPosition.x && headPosition.y == pointPosition.y)
    {
        generateRandomPosition();
        playerPoints++;
        return true;
    }

    return false;
}

void Game::handleInput()
{
    char input = readInput();

    Direction newDir = movementDirection;

    if (input == 'w' || input == 'W')
    {
        newDir = Direction::Up;
    }
    if (input == 's' || input == 'S')
    {
        newDir = Direction::Down;
    }
    if (input == 'a' || input == 'A')
    {
        newDir = Direction::Left;
    }
    if (input == 'd' || input == 'D')
    {
        newDir = Direction::Right;
    }

    if (!isNewDirOppositeDirection(newDir, movementDirection))
    {
        movementDirection = newDir;
    }
}

char Game::readInput()
{
    termios oldTerminal{};
    termios newTerminal{};

    tcgetattr(STDIN_FILENO, &oldTerminal);
    newTerminal = oldTerminal;

    newTerminal.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newTerminal);

    int oldFlags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldFlags | O_NONBLOCK);

    char input = '\0';
    read(STDIN_FILENO, &input, 1);

    tcsetattr(STDIN_FILENO, TCSANOW, &oldTerminal);
    fcntl(STDIN_FILENO, F_SETFL, oldFlags);

    return input;
}

void Game::generateRandomPosition()
{

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<int> xDist(1, width - 2);
    std::uniform_int_distribution<int> yDist(1, height - 2);

    int randomX;
    int randomY;
    do
    {
        randomX = xDist(gen);
        randomY = yDist(gen);

    } while (isSnakeBodyPosition(randomX, randomY) ||
             (randomX == headPosition.x && randomY == headPosition.y));

    pointPosition.x = randomX;
    pointPosition.y = randomY;
}

void Game::setDirection(Direction newDir)
{
    if (!isNewDirOppositeDirection(newDir, movementDirection))
    {
        pendingDirection = newDir;
    }
}