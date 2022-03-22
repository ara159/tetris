#include "Field.hpp"
#include "constants.hpp"

Field::Field()
{
    size[0] = COLUMNS;
    size[1] = LINES;
    margin[0] = BLOCK_SIZE;
    margin[1] = BLOCK_SIZE;
    grid_color = Color{60, 98, 158};
    bg_color = Color{0, 0, 0};
}

Field::~Field()
{
}

void Field::draw(RenderWindow* window, Block* blocks[COLUMNS][LINES], Piece turn)
{
    auto box = RectangleShape(Vector2f(size[0] * BLOCK_SIZE, size[1] * BLOCK_SIZE));
    box.setFillColor(bg_color);
    box.setPosition(margin[0], margin[1]);
    window->draw(box);

    RectangleShape line = RectangleShape();
    line.setFillColor(grid_color);
    
    // desenha os blocos
    Block block;
    Vector2i pos;
    int offset[2] = {margin[0] / BLOCK_SIZE, margin[1] / BLOCK_SIZE};

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

    // desenha o local onde o bloco irá cair
    Block bcopy[turn.blocks->size()];
    
    for (int i = 0; i < turn.blocks->size(); i++)
    {
        bcopy[i] = *turn.blocks->at(i);
    }

    while (true)
    {
        for (int i = 0; i < turn.blocks->size(); i++)
        {
            bcopy[i].move(0, 1);
        }
        
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
    }

    // desenha a peça atual
    for (auto tblock : *turn.blocks)
    {
        block = *tblock;
        pos = block.getPosition();
        if (pos.y < 0) continue;
        block.setPosition(pos.x + offset[0], pos.y + offset[1]);
        window->draw(block);
    }

    // desenha o grid horizontal
    for (int i = 0; i < size[1] + 1; i ++)
    {
        line.setSize(Vector2f(size[0] * BLOCK_SIZE, 2));
        line.setPosition(margin[0], margin[1] + i * BLOCK_SIZE);
        window->draw(line);
    }

    // desenha o grid vertical
    for (int i = 0; i < size[0] + 1; i ++)
    {
        line.setSize(Vector2f(2, size[1] * BLOCK_SIZE));
        line.setPosition(margin[0] + i * BLOCK_SIZE, margin[1]);
        window->draw(line);
    }
}
