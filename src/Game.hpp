#ifndef GAME_H
#define GAME_H 1
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <cmath>
#include "Block.hpp"
#include "Piece.hpp"
#include "constants.hpp"
#include "PredictArea.hpp"
#include "Field.hpp"
#include "Score.hpp"

class Game
{
private:
    int colldown;
    bool paused;
    Block* blocks[COLUMNS][LINES];
    sf::RenderWindow* window;
    Piece* turnForm;
    Piece* nextForm;
    bool debug_mode;
    bool force_drop;
    std::vector<int> lines_completeds;
    PredictArea predict;
    Field field;
    Score score;
    bool check_fall_collisions();
    void reset();
    void run_animations();
    void run();
    void game_logic();
    void draw();
    void rotate_pressed();
    void increase_velocity();
    void drop_pressed();
    void move_pressed(int direction);
    void event_handler();
    void create_form();
    bool check_collision();
public:
    Game(bool debug);
    ~Game();
    void start();
};

#endif
