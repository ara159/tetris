#include "Field.hpp"
#include "constants.hpp"

Field::Field() {}

Field::Field(Vector2i position, int block_size)
{
    this->position = position;
    this->block_size = block_size;

    grid_color = Color{60, 98, 158};
    bg_color = Color{0, 0, 0};
}

Field::~Field()
{
}

void Field::draw(RenderWindow* window, Block* blocks[COLUMNS][LINES], Piece turn)
{
    auto box = RectangleShape(Vector2f(COLUMNS * BLOCK_SIZE, LINES * BLOCK_SIZE));
    box.setFillColor(bg_color);
    box.setPosition(position.x, position.y);
    window->draw(box);

    RectangleShape line = RectangleShape();
    line.setFillColor(grid_color);
    
    // draws blocks
    Block block;
    Vector2i pos;
    auto box_pos = box.getPosition();
    int offset[2] = {(int) box_pos.x / BLOCK_SIZE, (int) box_pos.y / BLOCK_SIZE};

    for (int i = 0; i < COLUMNS; i++)
    {
        for (int j = 0; j < LINES; j++)
        {
            if (blocks[i][j] == nullptr)
                continue;
            
            block = *blocks[i][j];
            pos = block.getPosition();
            block.setPosition(pos.x + offset[0], pos.y + offset[1]);
            window->draw(block);
        }
    }

    // draws the block preview
    Block bcopy[turn.blocks->size()];
    
    for (int i = 0; i < turn.blocks->size(); i++)
    {
        bcopy[i] = *turn.blocks->at(i);
    }

    while (true)
    {
        bool stop = false;

        for (int i = 0; i < turn.blocks->size(); i++)
        {
            pos = bcopy[i].getPosition();
            if (pos.y >= LINES - 1 || blocks[pos.x][pos.y + 1] != nullptr)
            {
                stop = true;
                break;
            }
        }
        if (stop)
        {
            for (int i = 0; i < turn.blocks->size(); i++)
            {
                pos = bcopy[i].getPosition();
                if (pos.y < 0 || pos.y >= LINES || blocks[pos.x][pos.y] != nullptr) continue;
                bcopy[i].setPosition(pos.x + offset[0], pos.y + offset[1]);
                bcopy[i].setSize(Vector2f(bcopy[i].getSize().x, bcopy[i].getSize().y));
                auto c = bcopy->getFillColor();
                bcopy[i].setFillColor(Color{c.r, c.g, c.b, 28});
                window->draw(bcopy[i]);
            }
            break;
        }
        else
        {
            for (int i = 0; i < turn.blocks->size(); i++)
            {
                bcopy[i].move(0, 1);
            }        
        }
    }

    // draws current piece
    for (auto tblock : *turn.blocks)
    {
        block = *tblock;
        pos = block.getPosition();
        if (pos.y < 0) continue;
        block.setPosition(pos.x + offset[0], pos.y + offset[1]);
        window->draw(block);
    }

    // draws horizontal grid
    for (int i = 0; i < LINES + 1; i ++)
    {
        line.setSize(Vector2f(COLUMNS * BLOCK_SIZE, 2));
        line.setPosition(box_pos.x, box_pos.y + i * BLOCK_SIZE);
        window->draw(line);
    }

    // draws vertical grid
    for (int i = 0; i < COLUMNS + 1; i ++)
    {
        line.setSize(Vector2f(2, LINES * BLOCK_SIZE));
        line.setPosition(box_pos.x + i * BLOCK_SIZE, box_pos.y);
        window->draw(line);
    }
}
