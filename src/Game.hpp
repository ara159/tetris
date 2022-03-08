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

enum class AnimationType: int { LineCompleted };

class Animation
{
private:
public:
    int duration;
    int line;
    AnimationType anim_type;
    Animation(int duration, int line, AnimationType anim_type);
    ~Animation();
    void run();
    bool isCompleted();
};

class Game
{
private:
    int colldown;
    bool paused;
    Block* blocks[COLUMNS][LINES];
    void drawGrid(sf::RenderWindow* window);
    void drawBlocks(sf::RenderWindow* window);
    void addTurnToBlocks();
    bool checkFallCollisions();
    Piece* turnForm;
    bool debug_mode;
    bool force_drop;
    void reset();
    void runAnimations();
    std::vector<Animation> queue_animations;
public:
    Game(bool debug);
    ~Game();
    void fall();
    void draw(sf::RenderWindow* window);
    void pausePressed();
    void rotatePressed();
    void increaseVelocity();
    bool isPaused();
    void dropPressed();
    void movePressed(int direction);
    void checkLineComplete();
    void checkGameOver();
};

#endif
