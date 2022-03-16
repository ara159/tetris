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
    for (auto block : *blocks)
    {
        block->setFillColor(sf::Color{PIECE_COLOR});
    }
}

Piece::~Piece()
{
}

void Piece::randomCreate() {
    PieceType block_type = static_cast<PieceType>(rand() % 7);
    std::vector<int> format;

    switch (block_type)
    {
    case PieceType::L:
        format = std::vector<int>({
            0, 0,
            0, 1,
            0, 2,
            1, 2
        });
        origin = sf::Vector2f(0.5f, 1.5f);
        break;
    case PieceType::Cube:
        format = std::vector<int>({
            0, 0,
            0, 1,
            1, 0,
            1, 1
        });
        origin = sf::Vector2f(1, 1);
        break;
    case PieceType::Line:
        format = std::vector<int>({
            0, 1,
            0, 2,
            0, 3,
            0, 4
        });
        origin = sf::Vector2f(0, 3);
        break;
    case PieceType::LI:
        format = std::vector<int>({
            1, 0,
            1, 1,
            1, 2,
            0, 2
        });
        origin = sf::Vector2f(1.5f, 1.5f);
        break;
    case PieceType::T:
        format = std::vector<int>({
            1, 0,
            2, 0,
            3, 0,
            2, 1
        });
        origin = sf::Vector2f(2.5f, 0.5f);
        break;
    case PieceType::Z:
        format = std::vector<int>({
            0, 1,
            1, 1,
            1, 0,
            2, 0
        });
        origin = sf::Vector2f(1.5f, 1.5f);
        break;
    case PieceType::ZI:
        format = std::vector<int>({
            0, 0,
            1, 0,
            1, 1,
            2, 1
        });
        origin = sf::Vector2f(1.5f, 1.5f);
        break;
    }
    for (int i = 0; i < format.size(); i+=2)
    {
        auto block = new Block();
        block->setPosition(format.at(i), format.at(i+1));
        blocks->push_back(block);
        block->move(0, -4);
    }
    origin.y += -4;
}

void Piece::rotate() {
    int rotation[2][2] = {{0, 1}, {-1, 0}};

    for (int i = 0; i < blocks->size(); i++)
    {
        auto block = blocks->at(i);

        float pos[2] = {block->getPosition().x - origin.x, block->getPosition().y - origin.y};
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

        x += origin.x;
        y += origin.y;
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
    origin_point.setPosition(origin.x * BLOCK_SIZE, origin.y * BLOCK_SIZE);
    origin_point.setFillColor(sf::Color::Green);
    window->draw(origin_point);
}

void Piece::move(int x, int y)
{
    for (auto block : *blocks)
    {
        block->move(x, y);
    }
    origin.x += x;
    origin.y += y;
}