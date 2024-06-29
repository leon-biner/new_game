#pragma once
#include <ncurses.h>
#include <vector>
#include <string>

class Menu {
public:
    Menu(WINDOW*, std::vector<std::string> options, int number_of_options, std::vector<int> modes, int option_highlighted = 0);
    virtual void displayMenu() const = 0;
    virtual int chooseOption(); //returns the mode of the game + 1 depending on option chosen, returns 
    //0 if we stay in the same mode
protected:
    WINDOW* menu_window_;
    std::vector<std::string> options_;
    int option_highlighted_; //id of menu option chosen
    int number_of_options_;
    std::vector<int> modes_; //the index is the index of the menu option, element is mode associated with that choice
};

class MainMenu : public Menu {
public:
    MainMenu(WINDOW*, std::vector<std::string> options, int number_of_options, std::vector<int> modes, int option_highlighted = 0);
    void displayMenu() const override;
};

class InfoMenu : public Menu {
public:
    InfoMenu(WINDOW*, std::vector<std::string> options, int number_of_options, std::vector<int> modes, int option_highlighted = 0);
    void displayMenu() const override;
};

class PauseMenu : public Menu {
public:
    PauseMenu(WINDOW*, std::vector<std::string> options, int number_of_options, std::vector<int> modes, int option_highlighted = 0);
    void displayMenu() const override;
    int chooseOption() override;
};

class DeathMenu : public PauseMenu {
public:
    DeathMenu(WINDOW*, std::vector<std::string> options, int number_of_options, std::vector<int> modes, int option_highlighted = 0);
    void displayMenu() const override;
};