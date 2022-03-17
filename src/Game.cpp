#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <cmath>
#include "Block.hpp"
#include "Piece.hpp"
#include "constants.hpp"
#include "Game.hpp"
#include <string>

Game::Game(bool debug)
{
    this->paused = false;
    this->debug_mode = debug;
    this->lines_completeds = std::vector<int>();
    reset();
}

Game::~Game()
{
}

void Game::draw()
{
    const auto gridColor = sf::Color{78, 25, 118};
    const auto bgColor = sf::Color{BG_COLOR};
    
    // limpa a tela
    window->clear(bgColor);
    
    // // desenha o grid horizontal
    for (int i = 0; i < LINES; i ++)
    {
        auto v1 = sf::Vertex(sf::Vector2f(0, i * BLOCK_SIZE));
        auto v2 = sf::Vertex(sf::Vector2f(FIELD_W, i * BLOCK_SIZE));
        v1.color = v2.color = gridColor;
        sf::Vertex line[] = {v1, v2};
        window->draw(line, 2, sf::Lines);
    }

    // // desenha o grid vertical
    for (int i = 0; i < COLUMNS+1; i ++)
    {
        auto v1 = sf::Vertex(sf::Vector2f(i * BLOCK_SIZE, 0));
        auto v2 = sf::Vertex(sf::Vector2f(i * BLOCK_SIZE, FIELD_H));
        v1.color = v2.color = gridColor;
        sf::Vertex line[] = {v1, v2};
        window->draw(line, 2, sf::Lines);
    }
    
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

    drawPredict();

    // desenha informações de debug
    if (debug_mode) {
        sf::Font font;
        font.loadFromFile("arial.ttf");
        int i = 0;
        for (auto block : *turnForm->blocks)
        {
            auto pos = block->getPosition();
            auto rect = sf::RectangleShape(sf::Vector2f(BLOCK_SIZE, 2));
            rect.setPosition(pos.x * BLOCK_SIZE, pos.y * BLOCK_SIZE + BLOCK_SIZE);
            rect.setFillColor(sf::Color{230, 46, 0});
            window->draw(rect);

            
            auto text = sf::Text();
            text.setFont(font);
            text.setString(to_string(i));
            text.setCharacterSize(24);
            text.setFillColor(sf::Color::Red);
            text.setPosition(pos.x * BLOCK_SIZE, pos.y * BLOCK_SIZE);
            window->draw(text);
            i++;
        }
        turnForm->debug(window);
    }

    // exibe
    window->display();
}

void Game::drawPredict()
{
    const int p_block_size = 20;
    const int size_box = p_block_size * 5;

    auto box = sf::RectangleShape(sf::Vector2f(size_box, size_box));
    box.setFillColor(sf::Color{BG_COLOR});
    box.setOutlineColor(sf::Color::White);
    box.setOutlineThickness(1);
    box.setPosition(FIELD_W + EXTRA_W / 2, FIELD_W/2);
    box.setOrigin(sf::Vector2f(size_box/2, size_box/2));
    window->draw(box);

    auto rpos = box.getPosition();
    int offset_y = p_block_size * 1;
    int offset_x = p_block_size * 1;

    int min_x, min_y, max_x, max_y;
    min_x = min_y = INT32_MAX;
    max_x = max_y = INT32_MIN;

    for (auto block : *nextForm->blocks)
    {
        auto pos = block->getPosition();
        if(pos.x < min_x)
        {
            min_x = pos.x;
        }
        if(pos.x > max_x)
        {
            max_x = pos.x;
        }
        if(pos.y < min_y)
        {
            min_y = pos.y;
        }
        if(pos.y > max_y)
        {
            max_y = pos.y;
        }
    }

    int dif_x = max_x - min_x + 1;
    int dif_y = max_y - min_y + 1;

    std::cout << dif_x << "," << dif_y << std::endl;

    for (auto block : *nextForm->blocks)
    {
        auto bpos = block->getPosition();
        auto p_block = sf::RectangleShape(sf::Vector2f(p_block_size, p_block_size));
        p_block.setPosition(bpos.x * p_block_size + rpos.x - (dif_x * p_block_size)/2, bpos.y * p_block_size + rpos.y - (dif_y * p_block_size)/2);
        window->draw(p_block);
    }
}

void Game::gameLogic() {
    if (paused) return;
    
    // handle velocity using cooldown
    if (--colldown > 0) return;
    colldown = MAX_FALL_COOLDOWN;

    // if collide, handle some events
    if (checkFallCollisions()) {

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
        
        turnForm = nextForm;
        nextForm = new Piece();
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

        if (position.x < 0)
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
    turnForm->rotate();
}

void Game::dropPressed() {
    auto collide = checkFallCollisions();
    while (!collide)
    {
        turnForm->move(0, 1);
        collide = checkFallCollisions();
    }
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
    nextForm = new Piece();
    
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
    const int width = BLOCK_SIZE * COLUMNS + EXTRA_W;
    const int height = BLOCK_SIZE * LINES;
    window = new sf::RenderWindow(sf::VideoMode(width, height), "Tetris");
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
