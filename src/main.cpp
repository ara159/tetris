#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include "Game.hpp"

int main()
{
    const int block_size = 30;
    sf::RenderWindow window(sf::VideoMode(block_size * 10, block_size * 20), "My window");
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

    Game* game = new Game(true);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if ((event.type == sf::Event::KeyReleased) && (event.key.code == sf::Keyboard::Enter))
            {
                game->pausePressed();
            }
            if ((!game->isPaused() || true) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                game->movePressed(-1);
            }
            if ((!game->isPaused() || true) && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                game->movePressed(1);
            }
            if ((!game->isPaused() || true) && sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                game->dropPressed();
            }
            if ((!game->isPaused() || true) && (event.type == sf::Event::KeyReleased) && (event.key.code == sf::Keyboard::Space))
            {
                game->rotatePressed();
            }
        }
        window.clear(sf::Color{98, 45, 138});
        
        if (!game->isPaused()) {
            game->fall();
        }
        
        game->draw(&window);
        window.display();
    }
    return 0;
}