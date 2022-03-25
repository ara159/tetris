#ifndef PREDICT_H
#define PREDICT_H 1
#include <SFML/Graphics.hpp>
#include "Piece.hpp"
#include "constants.hpp"

using namespace sf;

class Predict
{
private:
    Piece next;
    int block_size;
    int box_area;
    Vector2i position;
    int size;
public:
    Predict(Vector2i position, int size);
    Predict();
    ~Predict();
    void draw(RenderWindow* window);
    void set_next_piece(Piece next);
};

#endif