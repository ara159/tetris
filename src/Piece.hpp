#ifndef PIECE_H
#include <SFML/Graphics.hpp>
#include "Block.hpp"

using namespace std;
using namespace sf;
enum class PieceType: int { L = 0, LI, T, Z, ZI, Line, Cube, CompleteLine };
enum class Rotation: int { CLOCKWISE, COUNTER_CLOCKWISE };

class Piece
{
private:
    void randomCreate();
public:
    Vector2f pivot;
    vector<Block*>* blocks;
    PieceType shape;
    Piece();
    ~Piece();
    void rotate(Rotation way);
    void debug(RenderWindow* window);
    void move(int x, int y);
};

#define PIECE_H 1
#endif