#ifndef FIELD_FH
#define FIELD_FH 1
#include <SFML/Graphics.hpp>
#include "Block.hpp"
#include "constants.hpp"
#include "Piece.hpp"

using namespace sf;

class Field
{
private:
    int size[2];
    int margin[2];
    Color grid_color;
    Color bg_color;
public:
    Field();
    ~Field();
    void draw(RenderWindow* window, Block* blocks[COLUMNS][LINES], Piece turn);
};

#endif