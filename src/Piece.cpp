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
    blocks = new vector<Block*>();
    style = static_cast<PieceStyle>(rand() % 7);
    rotation_state = 0;
    create_blocks();
}

Piece::~Piece()
{
}

void Piece::create_blocks() {
    vector<int> format;
    Color color;
    
    switch (style)
    {
    case PieceStyle::J:
        format = vector<int>({
            0, 1,
            1, 1,
            2, 1,
            0, 0
        });
        pivot = Vector2f(1.5f, 1.5f);
        break;
    case PieceStyle::L:
        format = vector<int>({
            0, 1,
            1, 1,
            2, 1,
            2, 0
        });
        pivot = Vector2f(1.5f, 1.5f);
        break;
    case PieceStyle::O:
        format = vector<int>({
            0, 0,
            0, 1,
            1, 0,
            1, 1
        });
        pivot = Vector2f(1, 1);
        break;
    case PieceStyle::I:
        format = vector<int>({
            0, 0,
            1, 0,
            2, 0,
            3, 0
        });
        pivot = Vector2f(2, 1);
        break;
    case PieceStyle::T:
        format = vector<int>({
            0, 1,
            1, 1,
            2, 1,
            1, 0
        });
        pivot = Vector2f(1.5f, 1.5f);
        break;
    case PieceStyle::Z:
        format = vector<int>({
            0, 0,
            1, 0,
            1, 1,
            2, 1
        });
        pivot = Vector2f(1.5f, 1.5f);
        break;
    case PieceStyle::S:
        format = vector<int>({
            0, 1,
            1, 1,
            1, 0,
            2, 0
        });
        pivot = Vector2f(1.5f, 1.5f);
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

    for (int i = 0; i < rand() % 4; i++)
    {
        rotate(Rotation::CLOCKWISE);
    }
}

void Piece::rotate(Rotation way) {
    if (style == PieceStyle::O)
        return;
    
    int rotation[2][2];
    
    rotation[0][0] = 0;
    rotation[1][1] = 0;

    if (way == Rotation::CLOCKWISE)
    {
        rotation[0][1] = -1;
        rotation[1][0] = 1;
    }

    if (way == Rotation::COUNTER_CLOCKWISE)
    {
        rotation[0][1] = 1;
        rotation[1][0] = -1;
    }

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
        if (way == Rotation::CLOCKWISE)
            block->move(-1, 0);
        else
            block->move(0, -1);
    }
    if (way == Rotation::CLOCKWISE)
        rotation_state = (rotation_state + 1) % 4;
    else
        rotation_state = (rotation_state - 1) % 4;
}

void Piece::debug(RenderWindow* window) {
    int points_radius = 2;
    auto origin_point = CircleShape(points_radius);
    origin_point.setOrigin(points_radius, points_radius);
    origin_point.setPosition(pivot.x * BLOCK_SIZE, pivot.y * BLOCK_SIZE);
    origin_point.setFillColor(Color::Green);
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

Vector2i Piece::size() 
{
    Vector2i pos;
    int ymax, xmax;
    int ymin, xmin;
    
    ymax = xmax = INT32_MIN;
    ymin = xmin = INT32_MAX;

    for (auto block : *blocks)
    {
        pos = block->getPosition();

        if (pos.x < xmin)
            xmin = pos.x;
        if (pos.x > xmax)
            xmax = pos.x;
        if (pos.y < ymin)
            ymin = pos.y;
        if (pos.y > ymax)
            ymax = pos.y;
    }

    return Vector2i(xmax - xmin, ymax - ymin);
}