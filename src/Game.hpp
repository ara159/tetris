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
    bool checkFallCollisions();
    void reset();
    void runAnimations();
    void run();
    void gameLogic();
    void draw();
    void rotatePressed();
    void increaseVelocity();
    void dropPressed();
    void movePressed(int direction);
    void eventHandler();
public:
    Game(bool debug);
    ~Game();
    void start();
};

#endif
