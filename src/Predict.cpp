#include "Predict.hpp"
#include "constants.hpp"

using namespace sf;

Predict::~Predict()
{
}

Predict::Predict() {}

Predict::Predict(Vector2i position, int size)
{
    this->position = position;
    this->size = size;
    this->block_size = size / 5;
}

void Predict::draw(RenderWindow* window)
{
    auto box = RectangleShape(Vector2f(size, size));
    box.setFillColor(Color{BG_COLOR});
    box.setOutlineColor(Color::White);
    box.setOutlineThickness(1);
    box.setPosition(position.x, position.y);
    window->draw(box);

    Vector2i spiece = next->size();
    int v_size = (spiece.y + 1) * block_size;
    int h_size = (spiece.x + 1) * block_size;

    for (auto block : *next->blocks)
    {
        auto pos = block->getPosition();
        
        // create shape
        auto b = RectangleShape(Vector2f(block_size - 2, block_size - 2));
        b.setFillColor(block->getFillColor());
        b.setPosition(
            pos.x * block_size + position.x + size - h_size - (size - h_size) / 2, // x
            pos.y * block_size + position.y + size - v_size - (size - v_size) / 2  // y
        );
        // some rotations demands adjusts on alignment
        if (next->rotation_state == 2)
        {
            b.setPosition(
                b.getPosition().x,
                b.getPosition().y - block_size
            );
        }
        if (next->rotation_state == 1)
        {
            b.setPosition(
                b.getPosition().x - block_size * (next->style == PieceStyle::I ? 2 : 1),
                b.getPosition().y + block_size * (next->style == PieceStyle::I ? 1 : 0)
            );
        }
        window->draw(b);
    }
}

void Predict::set_next_piece(Piece *next)
{
    this->next = next;
}
