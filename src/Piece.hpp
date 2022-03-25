#ifndef PIECE_H
#include <SFML/Graphics.hpp>
#include "Block.hpp"

using namespace std;
using namespace sf;
enum class PieceStyle: int { L = 0, J, T, Z, S, I, O };
enum class Rotation: int { CLOCKWISE, COUNTER_CLOCKWISE };

class Piece
{
private:
    void create_blocks();
public:
    Vector2f pivot;
    vector<Block*> blocks;
    PieceStyle style;
    int rotation_state;
    Piece();
    ~Piece();
    void rotate(Rotation way);
    void debug(RenderWindow* window);
    void move(int x, int y);
    Vector2i size();
};

#define PIECE_H 1
#endif