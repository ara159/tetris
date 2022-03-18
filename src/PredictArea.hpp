#ifndef PREDICT_H
#define PREDICT_H 1
#include <SFML/Graphics.hpp>
#include "Piece.hpp"
#include "constants.hpp"

using namespace sf;

class PredictArea
{
private:
    Piece* next;
    int block_size;
    int box_area;
    int box_pos[2];
public:
    PredictArea();
    ~PredictArea();
    void draw(RenderWindow* window);
    void setNextForm(Piece* next);
};

#endif