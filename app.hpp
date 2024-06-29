#pragma once
#include "menu.hpp"
#include "game.hpp"

class Application {
public:
    Application(Game game, std::map<std::string, std::shared_ptr<Menu>> menus, int mode = 0);
    void runApp();
    void navigateMenu(std::string menu_key);
    void playGame();
    void save();
private:
    int mode_; //mode 0:main menu, 1:info menu, 2:game, 3:pause menu, 4:death menu, 5:quit
    std::map<std::string, std::shared_ptr<Menu>> menus_; //keys : main, info, pause, death
    Game game_;
};