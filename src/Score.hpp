#ifndef SCORE_H
#define SCORE_H 1
#include "SFML/Graphics.hpp"

using namespace sf;

class Score
{
private:
    int level;
    int lines;
    int next_colldown;
    Font font;
    Vector2i position;
public:
    Score();
    Score(Vector2i position);
    ~Score();
    void up();
    void reset();
    int cooldown();
    void draw(RenderWindow* window);
};

#endif