#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <cmath>
#include "Block.hpp"
#include "Piece.hpp"
#include "constants.hpp"
#include "Game.hpp"
#include "Field.hpp"
#include <string>

Game::Game(bool debug)
{
    this->paused = false;
    this->debug_mode = debug;
    this->lines_completeds = std::vector<int>();
    this->predict = PredictArea();
    this->field = Field();
    reset();
}

Game::~Game()
{
}

void Game::draw()
{
    const auto bgColor = sf::Color{BG_COLOR};
    
    // limpa a tela
    window->clear(bgColor);

    field.draw(window, blocks, *turnForm);
    predict.draw(window);
    
    if (debug_mode)
        field.draw_debug(window, blocks, *turnForm);

    // exibe
    window->display();
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
        createForm();
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

bool Game::rotateCollision()
{
    for (auto block : *turnForm->blocks)
    {
        auto pos = block->getPosition();
        if (pos.x >= COLUMNS || pos.x < 0 || pos.y >= LINES || blocks[pos.x][pos.y] != nullptr)
        {
            return true;
        }
    }
    return false;
}

void Game::rotatePressed() {
    if (turnForm->style == PieceStyle::O)
    {
        return;
    }

    int s = turnForm->rotation_state;    
    turnForm->rotate(Rotation::CLOCKWISE);

    // no collisions, there's no need to use kicks
    if (!rotateCollision())
    { 
        return;
    }
    
    int tests[4][4][2];

    if (turnForm->style == PieceStyle::I)
    {
        int x[4][4] = {
            {-2, 1, -2, 1},
            {-1, 2, -1, 2},
            {2, -1, 2, -1},
            {1, -2, 1, -2}
        };
        int y[4][4] = {
            {0, 0, 1, -2},
            {0, 0, -2, 1},
            {0, 0, -1, 2},
            {0, 0, 2, 1}
        };
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                tests[i][j][0] = x[i][j];
                tests[i][j][1] = y[i][j];
            }
        }
    }
    else
    {
        int x[4][4] = {
            {-1, -1, 0, -1},
            {1, 1, 0, 1},
            {1, 1, 0, 1},
            {-1, -1, 0, -1}
        };
        int y[4][4] = {
            {0, -1, 2, 2},
            {0, 1, -2, -2},
            {0, -1, 2, 2},
            {0, 1, -2, -2}
        };
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                tests[i][j][0] = x[i][j];
                tests[i][j][1] = y[i][j];
            }
        }
    }

    for (int i = 0; i < 5; i++)
    {
        turnForm->move(tests[s][i][0], tests[s][i][1]);
        if (rotateCollision())
        {
            // fail - move to original position
            turnForm->move(-tests[s][i][0], -tests[s][i][1]);
        }
        else
        {
            return;
        }
    }

    // rotation fail
    turnForm->rotate(Rotation::COUNTER_CLOCKWISE);
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

void Game::createForm()
{
    if (nextForm == nullptr)
        turnForm = new Piece();
    else
        turnForm = nextForm;
    nextForm = new Piece();
    turnForm->move(COLUMNS/2-1, -4);
    predict.setNextForm(nextForm);
}

void Game::reset() {
    colldown = MAX_FALL_COOLDOWN;
    force_drop = false;
    createForm();
    
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
                    Uint8 r = color.r - (255 / (duration/tx));
                    Uint8 g = color.g - (255 / (duration/tx));
                    Uint8 b = color.b - (255 / (duration/tx));
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
    const int width = BLOCK_SIZE * (COLUMNS + 2) + EXTRA_W;
    const int height = BLOCK_SIZE * (LINES + 2);
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
        if (!paused || debug_mode) {
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
