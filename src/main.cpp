#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include "Game.hpp"

int main()
{
    Game* game = new Game(false);
    game->start();
    return 0;
}