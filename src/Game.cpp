#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <cmath>
#include "Block.hpp"
#include "Piece.hpp"
#include "Game.hpp"
#include "Field.hpp"
#include <string>

Game::Game()
{
    this->paused = false;
    this->lines_completeds = vector<int>();
    
    Vector2i pfield(BLOCK_SIZE, BLOCK_SIZE);
    Vector2i sfield(BLOCK_SIZE * COLUMNS, BLOCK_SIZE * LINES);
    Vector2i ppredi(pfield.x + sfield.x + BLOCK_SIZE, pfield.y + sfield.y * 0.1);
    Vector2i pscore(ppredi.x, ppredi.y + BLOCK_SIZE * 4);

    this->field = Field(pfield, BLOCK_SIZE);
    this->predict = Predict(ppredi, 100);
    this->score = Score(pscore);
    this->size = Vector2i(ppredi.x + 100 + BLOCK_SIZE, pfield.y + sfield.y + BLOCK_SIZE);
    this->prep_cdwn = Cooldown(30, false);

    // initialize game table
    for (int i = 0; i < COLUMNS; i++)
        for (int j = 0; j < LINES; j++)
            blocks[i][j] = nullptr;

    reset();
}

Game::~Game()
{
    clear_game_table();
}

void Game::draw()
{
    const auto bgColor = Color{0, 20, 43};
    
    // clear screen
    window->clear(bgColor);

    field.draw(window, blocks, actual);
    predict.draw(window);
    score.draw(window);
    
    // paused screen
    if (paused)
    {
        RectangleShape fg(Vector2f(size.x, size.y));
        fg.setFillColor(Color{0, 0, 0, 64});
        fg.setPosition(0, 0);
        window->draw(fg);

        auto font = Font();
        font.loadFromFile("Tetris.ttf");
        Text text("PAUSED", font);
        text.setCharacterSize(28);
        text.setPosition(size.x / 2 - text.getCharacterSize() * 3, size.y / 2);
        text.setStyle(Text::Bold);
        window->draw(text);
    }

    // show
    window->display();
}

void Game::game_logic() {
    if (paused)
        return;

    if (!prep_cdwn.check())
        return;
    
    // handle velocity using cooldown
    if (!fall_cdwn.check())
        return;
    
    // if collide, handle some events
    if (check_fall_collisions())
    {
        // add the turn block in matrix
        for (auto block : actual.blocks)
        {
            auto pos = block->getPosition();
            blocks[pos.x][pos.y] = block;
        }

        // check for lines completes
        for (int i = 0; i < LINES; i++)
        {
            bool complete = true;
            for (int j = 0; j < COLUMNS; j++)
            {
                complete = blocks[j][i] != nullptr && complete;
            }
            if (complete)
            {        
                score.up();
                fall_cdwn.set_max(score.cooldown());
                lines_completeds.push_back(i);
            }
        }
        
        // check for game over
        for (int i = 0; i < COLUMNS; i++)
        {
            if (blocks[i][0] != nullptr)
            {
                reset();
                return;
            }
        }
        create_form();
        return;
    }
    else {
        actual.move(0, 1);
    }
}

bool Game::check_fall_collisions() {
    for (auto block : actual.blocks)
    {
        auto pos = block->getPosition();

        // pre game
        if (pos.y < 0)
        {
            continue;
        }
        
        // touch on floor or other block
        if (pos.y == LINES - 1 || blocks[pos.x][pos.y + 1] != nullptr)
        {
            return true;
        }
    }
    return false;
}

bool Game::check_collision()
{
    for (auto block : actual.blocks)
    {
        auto pos = block->getPosition();
        if (pos.x >= COLUMNS || pos.x < 0 || pos.y >= LINES || blocks[pos.x][pos.y] != nullptr)
        {
            return true;
        }
    }
    return false;
}

void Game::rotate_pressed() {
    if (actual.style == PieceStyle::O)
    {
        return;
    }

    int s = actual.rotation_state;    
    actual.rotate(Rotation::CLOCKWISE);

    // no collisions, there's no need to use kicks
    if (!check_collision())
    { 
        return;
    }
    
    int tests[4][4][2];

    if (actual.style == PieceStyle::I)
    {
        int x[4][4] = {
            {-2, 1, -2, 1},
            {-1, 2, -1, 2},
            {2, -1, 2, -1},
            {1, -2, 1, -2}
        };
        int y[4][4] = {
            {0, 0, 1, -2},
            {0, 0, -2, 1},
            {0, 0, -1, 2},
            {0, 0, 2, 1}
        };
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                tests[i][j][0] = x[i][j];
                tests[i][j][1] = y[i][j];
            }
        }
    }
    else
    {
        int x[4][4] = {
            {-1, -1, 0, -1},
            {1, 1, 0, 1},
            {1, 1, 0, 1},
            {-1, -1, 0, -1}
        };
        int y[4][4] = {
            {0, -1, 2, 2},
            {0, 1, -2, -2},
            {0, -1, 2, 2},
            {0, 1, -2, -2}
        };
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                tests[i][j][0] = x[i][j];
                tests[i][j][1] = y[i][j];
            }
        }
    }

    for (int i = 0; i < 5; i++)
    {
        actual.move(tests[s][i][0], tests[s][i][1]);
        if (check_collision())
        {
            // fail - move to original position
            actual.move(-tests[s][i][0], -tests[s][i][1]);
        }
        else
        {
            return;
        }
    }

    // rotation fail
    actual.rotate(Rotation::COUNTER_CLOCKWISE);
}

void Game::move_pressed(int direction) {
    Vector2i position;
    Block *candidate;
    
    for (auto block : actual.blocks)
    {
        position = block->getPosition();

        // touch wall
        if (position.x <= 0 && direction < 0) return;
        if (position.x + 1 >= COLUMNS && direction > 0) return;

        // pre game
        if (position.y < 0) continue;

        candidate = blocks[position.x + direction][position.y];

        if (candidate == nullptr) continue;
        
        // other block collision
        if (candidate->getPosition().x == (position.x - 1))
            return;
        
        // other block collision
        if (position.x + 1 == candidate->getPosition().x)
            return;
    }
    actual.move(direction, 0);
}

void Game::create_form()
{
    actual = next;
    next = Piece();
    actual.move(COLUMNS/2-1, - actual.size().y - 2);
    predict.set_next_piece(next);
    prep_cdwn.reset();
}

void Game::reset() {
    actual = Piece();
    next = Piece();
    score.reset();
    prep_cdwn.reset();
    fall_cdwn.set_max(score.cooldown());
    fall_cdwn.reset();
    create_form();
    clear_game_table();
}

void Game::clear_game_table()
{
    for (int i = 0; i < COLUMNS; i++)
    {
        for (int j = 0; j < LINES; j++)
        {
            if (blocks[i][j] != nullptr) 
            {
                free(blocks[i][j]);
                blocks[i][j] = nullptr;
            }
        }
    }
}

void Game::run_animations() {
    // complete line animation
    if (lines_completeds.size() > 0)
    {
        int line = lines_completeds.front();
        int duration = 10;
        int tx = duration/2;

        // faz a animação acontecer e pinta na tela
        for (int i = 0; i < duration; i++)
        {
            if ((duration - i) % tx == 0)
            {
                for (int j = 0; j < COLUMNS; j++)
                {
                    auto block = blocks[j][line];
                    auto color = block->getFillColor();
                    Uint8 r = color.r - (255 / (duration/tx));
                    Uint8 g = color.g - (255 / (duration/tx));
                    Uint8 b = color.b - (255 / (duration/tx));
                    block->setFillColor(Color{r, g, b});
                }
            }
            draw();
        }
        
        // run after animation logic

        // free blocks memory
        for (int j = 0; j < COLUMNS; j++)
        {
            free(blocks[j][line]);
        }

        for (int i = 0; i < COLUMNS; i++)
        {
            for (int j = line; j > 0; j--)
            {
                if (blocks[i][j-1] != nullptr) {
                    blocks[i][j-1]->move(0, 1);
                }
                blocks[i][j] = blocks[i][j-1];
            }
        }

        lines_completeds.erase(lines_completeds.begin());
    }
}

void Game::start() {
    window = new RenderWindow(VideoMode(size.x, size.y), "Tetris");
    window->setVerticalSyncEnabled(true);
    window->setFramerateLimit(60);
    run();
    clear_game_table();
}

void Game::run() {
    while (window->isOpen())
    {
        game_logic();
        event_handler();
        draw();
        run_animations();
    }
}

void Game::event_handler() {
    Event event;
    
    while (window->pollEvent(event))
    {
        if (event.type == Event::Closed)
        {
            window->close();
        }
        if ((event.type == Event::KeyReleased) && (event.key.code == Keyboard::Enter))
        {
            paused = !paused;
        }
        if (!paused && event.type == Event::KeyPressed)
        {
            if (event.key.code == Keyboard::Left)
                move_pressed(-1);
            
            if (event.key.code == Keyboard::Right)
                move_pressed(1);

            if (event.key.code == Keyboard::Space)
                rotate_pressed();
        }
    }
    if (!paused) {
        if (Keyboard::isKeyPressed(Keyboard::Down))
        {
            fall_cdwn.set_remaining(1);
        }
    }
}
