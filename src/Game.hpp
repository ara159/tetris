#ifndef GAME_H
#define GAME_H 1

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "Block.hpp"
#include "Piece.hpp"
#include "Field.hpp"
#include "Score.hpp"
#include "Predict.hpp"
#include "Cooldown.hpp"
#include "Constants.hpp"

using namespace sf;
using namespace std;

class Game
{
private:
    Cooldown fall_cdwn;
    Cooldown prep_cdwn;
    bool paused;
    Block* blocks[COLUMNS][LINES];
    RenderWindow* window;
    Piece actual;
    Piece next;
    vector<int> lines_completeds;
    Predict predict;
    Field field;
    Score score;
    Vector2i size;
    
    bool check_fall_collisions();
    void reset();
    void run_animations();
    void run();
    void game_logic();
    void draw();
    void rotate_pressed();
    void increase_velocity();
    void move_pressed(int direction);
    void event_handler();
    void create_form();
    bool check_collision();
    void clear_game_table();
public:
    Game();
    ~Game();
    void start();
};

#endif
