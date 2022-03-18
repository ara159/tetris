#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <math.h>
#include "Block.hpp"
#include "Piece.hpp"
#include "constants.hpp"

Piece::Piece()
{
    this->blocks = new std::vector<Block*>();
    randomCreate();
}

Piece::~Piece()
{
}

void Piece::randomCreate() {
    PieceType block_type = static_cast<PieceType>(rand() % 7);
    std::vector<int> format;
    Color color;

    switch (block_type)
    {
    case PieceType::L:
        format = std::vector<int>({
            0, 0,
            0, 1,
            0, 2,
            1, 2
        });
        pivot = sf::Vector2f(0.5f, 1.5f);
        break;
    case PieceType::Cube:
        format = std::vector<int>({
            0, 0,
            0, 1,
            1, 0,
            1, 1
        });
        pivot = sf::Vector2f(1, 1);
        break;
    case PieceType::Line:
        format = std::vector<int>({
            0, 0,
            0, 1,
            0, 2,
            0, 3
        });
        pivot = sf::Vector2f(0, 2);
        break;
    case PieceType::LI:
        format = std::vector<int>({
            1, 0,
            1, 1,
            1, 2,
            0, 2
        });
        pivot = sf::Vector2f(1.5f, 1.5f);
        break;
    case PieceType::T:
        format = std::vector<int>({
            0, 0,
            1, 0,
            2, 0,
            1, 1
        });
        pivot = sf::Vector2f(1.5f, 0.5f);
        break;
    case PieceType::Z:
        format = std::vector<int>({
            0, 1,
            1, 1,
            1, 0,
            2, 0
        });
        pivot = sf::Vector2f(1.5f, 1.5f);
        break;
    case PieceType::ZI:
        format = std::vector<int>({
            0, 0,
            1, 0,
            1, 1,
            2, 1
        });
        pivot = sf::Vector2f(1.5f, 1.5f);
        break;
    }

    switch (rand() % 5)
    {
    case 0:
        color = Color{251, 255, 36};
        break;
    case 1:
        color = Color{255, 120, 36};
        break;
    case 2:
        color = Color{35, 212, 19};
        break;
    case 3:
        color = Color{0, 229, 255};
        break;
    case 4:
        color = Color{255, 0, 25};
        break;
    }

    for (int i = 0; i < format.size(); i+=2)
    {
        auto block = new Block();
        block->setPosition(format.at(i), format.at(i+1));
        block->setFillColor(color);
        blocks->push_back(block);
    }
}

void Piece::rotate() {
    int rotation[2][2] = {{0, 1}, {-1, 0}};

    for (int i = 0; i < blocks->size(); i++)
    {
        auto block = blocks->at(i);

        float pos[2] = {block->getPosition().x - pivot.x, block->getPosition().y - pivot.y};
        float x = 0;
        float y = 0;
        
        for (int j = 0; j < 2; j++)
        {
            x += rotation[0][j] * pos[j];
        }
        
        for (int j = 0; j < 2; j++)
        {
            y += rotation[1][j] * pos[j];
        }

        x += pivot.x;
        y += pivot.y;
        block->setPosition(x, y);
    }
    for (auto block : *blocks)
    {
        block->move(0, -1);
    }
}

void Piece::debug(sf::RenderWindow* window) {
    int points_radius = 2;
    auto origin_point = sf::CircleShape(points_radius);
    origin_point.setOrigin(points_radius, points_radius);
    origin_point.setPosition(pivot.x * BLOCK_SIZE, pivot.y * BLOCK_SIZE);
    origin_point.setFillColor(sf::Color::Green);
    window->draw(origin_point);
}

void Piece::move(int x, int y)
{
    for (auto block : *blocks)
    {
        block->move(x, y);
    }
    pivot.x += x;
    pivot.y += y;
}