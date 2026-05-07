#include <iostream>
#include <memory>

#include "snake/Game.hpp"

// int argc, char const *argv[]
int main()
{
    std::cout << "STARTED \n";

    std::unique_ptr<Game> game = std::make_unique<Game>();
    game->run();

    return 0;
}
