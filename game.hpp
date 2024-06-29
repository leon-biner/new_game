#pragma once
#include <ncurses.h>
#include <thread>
#include "main_header.hpp"
#include "characters.hpp"

class Game {
public:
    Game(std::vector<std::shared_ptr<LevelGrid>>, PlayerCharacter&, WINDOW*, bool game_over = false, bool pause = false, int current_lvl = 0);
    int renderGame(); //returns next mode
    void updateGame(); //here we need to check each enemy and player and shit for speed
    void clearWindow();
    void initialisePlayer();
private:
    int current_lvl_; //index of the current level;
    std::vector<std::shared_ptr<LevelGrid>> levels_;
    PlayerCharacter& player_;
    WINDOW* game_window_;
    bool game_over_;
    bool paused_;
};