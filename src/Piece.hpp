#ifndef PIECE_H
#include <SFML/Graphics.hpp>
#include "Block.hpp"

using namespace std;
using namespace sf;
enum class PieceType: int { L = 0, LI, T, Z, ZI, Line, Cube, CompleteLine };

class Piece
{
private:
    void randomCreate();
    Vector2f origin;
public:
    vector<Block*>* blocks;
    Piece();
    ~Piece();
    void rotate();
    void debug(RenderWindow* window);
    void move(int x, int y);
};

#define PIECE_H 1
#endif