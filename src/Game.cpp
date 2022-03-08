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
    this->queue_animations = std::vector<Animation>();
    reset();
}

Game::~Game()
{
}

void Game::draw(sf::RenderWindow* window)
{
    runAnimations();
    for (auto block : *turnForm->blocks)
    {
        window->draw(*block);
    }
    if (debug_mode) {
        turnForm->debug(window);
    }
    drawBlocks(window);
    drawGrid(window);
    if (debug_mode) {
        for (auto block : *turnForm->blocks)
        {
            auto pos = block->getPosition();
            auto rect = sf::RectangleShape(sf::Vector2f(BLOCK_SIZE, 2));
            rect.setPosition(pos.x * BLOCK_SIZE, pos.y * BLOCK_SIZE + BLOCK_SIZE);
            rect.setFillColor(sf::Color{230, 46, 0});
            window->draw(rect);
        }
    }
}

void Game::pausePressed()
{
    this->paused = !this->paused;
}

void Game::fall() {
    if (!queue_animations.empty()) return;
    colldown--;
    
    if (colldown > 0)
        return;
    
    colldown = MAX_FALL_COOLDOWN;

    bool collide = checkFallCollisions();
    while (force_drop && !collide)
    {
        turnForm->move(0, 1);
        collide = checkFallCollisions();
    }
    
    force_drop = false;

    if (collide) {
        addTurnToBlocks();
        checkGameOver();
        checkLineComplete();
        turnForm = new Piece();
        return;
    }
    turnForm->move(0, 1);
}

void Game::checkGameOver() {
    for (int i = 0; i < COLUMNS; i++)
    {
        // game over condition
        if (blocks[i][0] != nullptr)
        {
            reset();
            return;
        }
    }
}

void Game::checkLineComplete() {
    for (int i = 0; i < LINES; i++)
    {
        bool complete = true;
        for (int j = 0; j < COLUMNS; j++)
        {
            complete = blocks[j][i] != nullptr && complete;
        }
        if (complete)
        {            
            queue_animations.push_back(Animation(60, i, AnimationType::LineCompleted));
            return;
        }
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

void Game::addTurnToBlocks() {
    for (auto block : *turnForm->blocks)
    {
        auto pos = block->getPosition();
        blocks[pos.x][pos.y] = block;
    }
}

void Game::drawBlocks(sf::RenderWindow* window) {
    for (int i = 0; i < COLUMNS; i++)
    {
        for (int j = 0; j < LINES; j++)
        {
            if (blocks[i][j] == nullptr) continue; 
            window->draw(*blocks[i][j]);
        }
    }
}

void Game::drawGrid(sf::RenderWindow* window) {
    const auto color = sf::Color{17, 0, 28};

    for (int i = 0; i < LINES; i ++)
    {
        auto v1 = sf::Vertex(sf::Vector2f(0, i * BLOCK_SIZE));
        auto v2 = sf::Vertex(sf::Vector2f(FIELD_W, i * BLOCK_SIZE));
        v1.color = v2.color = color;
        sf::Vertex line[] = {v1, v2};
        window->draw(line, 2, sf::Lines);
    }

    for (int i = 0; i < COLUMNS; i ++)
    {
        auto v1 = sf::Vertex(sf::Vector2f(i * BLOCK_SIZE, 0));
        auto v2 = sf::Vertex(sf::Vector2f(i * BLOCK_SIZE, FIELD_H));
        v1.color = v2.color = color;
        sf::Vertex line[] = {v1, v2};
        window->draw(line, 2, sf::Lines);
    }
}

void Game::rotatePressed() {
    turnForm->rotate(M_PI/2);
}

bool Game::isPaused() {
    return paused;
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
            // free(blocks[i][j]);
            blocks[i][j] = nullptr;
        }
    }
}

void Game::runAnimations() {
    if (queue_animations.size() <= 0)
        return;
    
    auto turn = &queue_animations.front();
    
    switch (turn->anim_type)
    {
    case AnimationType::LineCompleted:
        for (int i = 0; i < COLUMNS; i++)
        {
            if (turn->duration % MAX_FALL_COOLDOWN == 0) {
                auto block = blocks[i][turn->line];
                auto color = block->getFillColor();
                
                Uint8 r = color.r - ((255 - 98) / 3);
                Uint8 g = color.g - ((255 - 45) / 3);
                Uint8 b = color.b - ((255 - 138) / 3);
                
                std::cout << (uint) r << "," << (uint) g << "," << (uint) b << std::endl;
                
                block->setFillColor(Color{r, g, b});
            }
        }
        break;
    }
    
    turn->run();
    
    if (turn->isCompleted())
    {
        queue_animations.erase(queue_animations.begin());
        switch (turn->anim_type)
        {
        case AnimationType::LineCompleted:
            for (int i = 0; i < COLUMNS; i++)
            {
                for (int j = turn->line; j > 0; j--)
                {
                    if (blocks[i][j-1] != nullptr) {
                        blocks[i][j-1]->move(0, 1);
                    }
                    blocks[i][j] = blocks[i][j-1];
                }
            }
            checkLineComplete();
            break;
        }
    }
}

Animation::Animation(int duration, int line, AnimationType anim_type)
{
    this->duration = duration;
    this->line = line;
    this->anim_type = anim_type;
}

Animation::~Animation()
{
}

void Animation::run() {
    this->duration--;
}

bool Animation::isCompleted() 
{
    return duration <= 0;
}