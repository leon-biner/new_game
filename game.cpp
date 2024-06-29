#include "game.hpp"

Game::Game(std::vector<std::shared_ptr<LevelGrid>> levels, PlayerCharacter& player, WINDOW* win, bool game_over
, bool pause, int current_lvl) : levels_(levels), player_(player), game_window_(win), game_over_(game_over)
, paused_(pause), current_lvl_(current_lvl) {} 

void Game::updateGame()
{
    int input = wgetch(game_window_);
    if (input != ERR)
        player_.move(*levels_[current_lvl_], input);
    if (input == (int)'p')
        paused_ = true;
    levels_[current_lvl_]->updateButtonsAndDoors();
    levels_[current_lvl_]->updateEnemies();
    if (player_.detectHit(*levels_[current_lvl_]))
        game_over_ = true;
    if (player_.goToNextLevel(*levels_[current_lvl_]))
    {
        current_lvl_++;
        if (current_lvl_ == levels_.size())
        {
            game_over_ = true;
            return;
        }
        player_.resetPos(*levels_[current_lvl_]);
    }
}

int Game::renderGame()
{
    if (game_over_) //this will not be the game_over menu but the player won window
    {
        game_over_ = false;
        return 4;
    }
    levels_[current_lvl_]->displayWalls(game_window_);
    levels_[current_lvl_]->displayEntities(game_window_);
    player_.display(game_window_);
    wrefresh(game_window_);
    if (game_over_)
    {
        game_over_ = false;
        return 4;
    }
    if (paused_)
    {
        paused_ = false;
        return 3;
    }
    return 2;
}

void Game::clearWindow()
{
    wclear(game_window_);
    wrefresh(game_window_);
}

void Game::initialisePlayer()
{
    player_.resetPos(*levels_[current_lvl_]);
}