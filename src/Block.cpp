#include <SFML/Graphics.hpp>
#include "Block.hpp"
#include "Game.hpp"

using namespace sf;
using namespace std;

Block::Block() : RectangleShape(Vector2f(BLOCK_SIZE, BLOCK_SIZE))
{
}

Block::~Block()
{
}

Vector2i Block::getPosition()
{
    auto p = RectangleShape::getPosition();
    return Vector2i(p.x/BLOCK_SIZE, p.y/BLOCK_SIZE);
}

void Block::move(float x, float y)
{
    RectangleShape::move(x * BLOCK_SIZE, y * BLOCK_SIZE);
}

void Block::setPosition(float x, float y)
{
    RectangleShape::setPosition(x * BLOCK_SIZE, y * BLOCK_SIZE);
}