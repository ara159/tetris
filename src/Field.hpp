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
    Vector2i position;
    Color grid_color;
    Color bg_color;
    int block_size;
public:
    Field();
    Field(Vector2i position, int block_size);
    ~Field();
    void draw(RenderWindow* window, Block* blocks[COLUMNS][LINES], Piece turn);
};

#endif