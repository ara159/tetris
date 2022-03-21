#include "PredictArea.hpp"
#include "constants.hpp"

using namespace sf;

PredictArea::~PredictArea()
{
}

PredictArea::PredictArea()
{
    box_area = 100;
    block_size = 20;
    box_pos[0] = FIELD_W + EXTRA_W / 2 + BLOCK_SIZE * 1.5;
    box_pos[1] = FIELD_H * 0.2f;
}

void PredictArea::draw(RenderWindow* window)
{
    auto box = RectangleShape(Vector2f(box_area, box_area));
    box.setFillColor(Color{BG_COLOR});
    box.setOutlineColor(Color::White);
    box.setOutlineThickness(1);
    box.setPosition(box_pos[0], box_pos[1]);
    box.setOrigin(Vector2f(box_area/2, box_area/2));
    window->draw(box);

    int min[2], max[2], diff[2], offset[2];

    min[0] = min[1] = INT32_MAX;
    max[0] = max[1] = INT32_MIN;

    for (auto block : *next->blocks)
    {
        auto pos = block->getPosition();
        if(pos.x < min[0])
        {
            min[0] = pos.x;
        }
        if(pos.x > max[0])
        {
            max[0] = pos.x;
        }
        if(pos.y < min[1])
        {
            min[1] = pos.y;
        }
        if(pos.y > max[1])
        {
            max[1] = pos.y;
        }
    }

    diff[0] = max[0] - min[0] + 1;
    diff[1] = max[1] - min[1] + 1;

    for (auto block : *next->blocks)
    {
        auto pos = block->getPosition();
        
        // create shape
        auto b = RectangleShape(Vector2f(block_size, block_size));
        b.setFillColor(block->getFillColor());
        b.setOutlineThickness(2);
        b.setOutlineColor(Color{BG_COLOR});

        offset[0] = box_pos[0] - (diff[0] * block_size)/2;
        offset[1] = box_pos[1] - (diff[1] * block_size)/2;
        b.setPosition(pos.x * block_size + offset[0], pos.y * block_size + offset[1]);
        window->draw(b);
    }
}

void PredictArea::setNextForm(Piece *next)
{
    this->next = next;
}