#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <cmath>
#include "Block.hpp"
#include "Piece.hpp"
#include "constants.hpp"
#include "Game.hpp"

Game::Game(bool debug)
{
    this->paused = false;
    this->debug_mode = debug;
    this->frozen = false;
    this->lines_completeds = std::vector<int>();
    reset();
}

Game::~Game()
{
}

void Game::draw()
{
    const auto gridColor = sf::Color{17, 0, 28};
    const auto bgColor = sf::Color{98, 45, 138};
    
    // limpa a tela
    window->clear(bgColor);
    
    // desenha os blocos do objeto em queda
    for (auto block : *turnForm->blocks)
    {
        window->draw(*block);
    }

    // desenha os blocos
    for (int i = 0; i < COLUMNS; i++)
    {
        for (int j = 0; j < LINES; j++)
        {
            if (blocks[i][j] == nullptr) continue; 
            window->draw(*blocks[i][j]);
        }
    }

    // desenha o grid horizontal
    for (int i = 0; i < LINES; i ++)
    {
        auto v1 = sf::Vertex(sf::Vector2f(0, i * BLOCK_SIZE));
        auto v2 = sf::Vertex(sf::Vector2f(FIELD_W, i * BLOCK_SIZE));
        v1.color = v2.color = gridColor;
        sf::Vertex line[] = {v1, v2};
        window->draw(line, 2, sf::Lines);
    }

    // desenha o grid vertical
    for (int i = 0; i < COLUMNS; i ++)
    {
        auto v1 = sf::Vertex(sf::Vector2f(i * BLOCK_SIZE, 0));
        auto v2 = sf::Vertex(sf::Vector2f(i * BLOCK_SIZE, FIELD_H));
        v1.color = v2.color = gridColor;
        sf::Vertex line[] = {v1, v2};
        window->draw(line, 2, sf::Lines);
    }
    
    // desenha informações de debug
    if (debug_mode) {
        for (auto block : *turnForm->blocks)
        {
            auto pos = block->getPosition();
            auto rect = sf::RectangleShape(sf::Vector2f(BLOCK_SIZE, 2));
            rect.setPosition(pos.x * BLOCK_SIZE, pos.y * BLOCK_SIZE + BLOCK_SIZE);
            rect.setFillColor(sf::Color{230, 46, 0});
            window->draw(rect);
        }
        turnForm->debug(window);
    }

    // exibe
    window->display();
}

void Game::gameLogic() {
    if (paused) return;
    if (frozen) return;
    
    // handle velocity using cooldown
    if (--colldown > 0) return;
    colldown = MAX_FALL_COOLDOWN;

    // the logic here is move the turn block down
    bool collide = checkFallCollisions();
    while (force_drop && !collide)
    {
        turnForm->move(0, 1);
        collide = checkFallCollisions();
    }
    force_drop = false;
        
    // if collide, handle some events
    if (collide) {

        // add the turn block in matrix
        for (auto block : *turnForm->blocks)
        {
            auto pos = block->getPosition();
            blocks[pos.x][pos.y] = block;
        }

        // check for lines completes
        for (int i = 0; i < LINES; i++)
        {
            bool complete = true;
            for (int j = 0; j < COLUMNS; j++)
            {
                complete = blocks[j][i] != nullptr && complete;
            }
            if (complete)
            {        
                lines_completeds.push_back(i);
            }
        }
        
        // check for game over
        for (int i = 0; i < COLUMNS; i++)
        {
            if (blocks[i][0] != nullptr)
            {
                reset();
                return;
            }
        }
        
        turnForm = new Piece();
        return;
    }
    else {
        turnForm->move(0, 1);
    }
}

bool Game::checkFallCollisions() {
    sf::Vector2i position;
    Block* candidate;

    for (auto block : *turnForm->blocks)
    {
        position = block->getPosition();

        // pre game
        if (position.y < 0)
            continue; 

        // touch on floor
        if (LINES - position.y == 1)
            return true;
        
        candidate = blocks[position.x][position.y + 1];
        
        if (candidate == nullptr)
            continue;
        
        // touch another block
        if (candidate->getPosition().y == (position.y + 1))
            return true;
    }
    return false;
}

void Game::rotatePressed() {
    turnForm->rotate(M_PI/2);
}

void Game::dropPressed() {
    force_drop = true;
}

void Game::movePressed(int direction) {
    sf::Vector2i position;
    Block *candidate;
    
    for (auto block : *turnForm->blocks)
    {
        position = block->getPosition();

        // touch wall
        if (position.x <= 0 && direction < 0) return;
        if (position.x + 1 >= COLUMNS && direction > 0) return;

        // pre game
        if (position.y < 0) continue;

        candidate = blocks[position.x + direction][position.y];

        if (candidate == nullptr) continue;
        
        // other block collision
        if (candidate->getPosition().x == (position.x - 1))
            return;
        
        // other block collision
        if (position.x + 1 == candidate->getPosition().x)
            return;
    }
    turnForm->move(direction, 0);
}

void Game::reset() {
    colldown = MAX_FALL_COOLDOWN;
    force_drop = false;
    turnForm = new Piece();
    
    for (int i = 0; i < COLUMNS; i++)
    {
        for (int j = 0; j < LINES; j++)
        {
            blocks[i][j] = nullptr;
        }
    }
}

void Game::runAnimations() {
    // animação de linha completa
    if (lines_completeds.size() > 0)
    {
        int line = lines_completeds.front();
        int duration = 10;
        int tx = duration/2;

        // faz a animação acontecer e pinta na tela
        for (int i = 0; i < duration; i++)
        {
            if ((duration - i) % tx == 0)
            {
                for (int j = 0; j < COLUMNS; j++)
                {
                    auto block = blocks[j][line];
                    auto color = block->getFillColor();
                    Uint8 r = color.r - ((255 - 98) / (duration/tx));
                    Uint8 g = color.g - ((255 - 45) / (duration/tx));
                    Uint8 b = color.b - ((255 - 138) / (duration/tx));
                    block->setFillColor(Color{r, g, b});
                }
            }
            draw();
        }

        // faz a lógica necessária após a animação
        for (int i = 0; i < COLUMNS; i++)
        {
            for (int j = line; j > 0; j--)
            {
                if (blocks[i][j-1] != nullptr) {
                    blocks[i][j-1]->move(0, 1);
                }
                blocks[i][j] = blocks[i][j-1];
            }
        }

        lines_completeds.erase(lines_completeds.begin());
    }
}

void Game::start() {
    window = new sf::RenderWindow(sf::VideoMode(BLOCK_SIZE * COLUMNS, BLOCK_SIZE * LINES), "Tetris");
    window->setVerticalSyncEnabled(true);
    window->setFramerateLimit(60);
    run();
}

void Game::run() {
    while (window->isOpen())
    {
        eventHandler();
        runAnimations();
        gameLogic();
        draw();
    }
}

void Game::eventHandler() {
    sf::Event event;
    while (window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window->close();
        }
        if ((event.type == sf::Event::KeyReleased) && (event.key.code == sf::Keyboard::Enter))
        {
            paused = !paused;
        }
        if (!paused) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                movePressed(-1);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                movePressed(1);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                dropPressed();
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
            {
                rotatePressed();
            }
        }
    }
}
