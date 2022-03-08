#ifndef BLOCK_H
#include <SFML/Graphics.hpp>

using namespace sf;

class Block : public RectangleShape
{
private:
    /* data */
public:
    Block();
    ~Block();
    Vector2i getPosition();
    void move(float x, float y);
    void setPosition(float x, float y);
};

#define BLOCK_H 1
#endif