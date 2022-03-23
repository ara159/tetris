#include "Score.hpp"
#include "SFML/Graphics.hpp"
#include "iostream"
#include "constants.hpp"
#include <string>

Score::Score() {}

Score::Score(Vector2i position)
{
    font = Font();
    if (!font.loadFromFile("Tetris.ttf"))
    {
        std::cout << "Error on loading file 'arial.ttf'" << std::endl;
    }
    this->position = position;
    reset();
}

Score::~Score()
{
}

void Score::up()
{
    lines++;

    int new_level = lines / 10;
    
    if (new_level == level)
    {
        return;
    }
    
    level = new_level;
    
    if (level < 8)
    {
        next_colldown -= 5;
    }
    else
    if (level < 10)
    {
        next_colldown -= 2;
    }
    else
    if (next_colldown > 1)
    {
        next_colldown -= 1;
    }
}

void Score::reset()
{
    level = 0;
    lines = 0;
    next_colldown = 48;
}

int Score::cooldown()
{
    return next_colldown;
}

void Score::draw(RenderWindow* window)
{
    Text text_lines("LINES: " + std::to_string(lines), font);
    text_lines.setCharacterSize(18);
    text_lines.setPosition(position.x, position.y);
    text_lines.setStyle(Text::Bold);
    window->draw(text_lines);

    Text text_level("LEVEL: " + std::to_string(level), font);
    text_level.setCharacterSize(18);
    text_level.setPosition(position.x, position.y + 20);
    text_level.setStyle(Text::Bold);
    window->draw(text_level);
}
