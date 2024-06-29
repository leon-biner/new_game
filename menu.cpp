#include "menu.hpp"

Menu::Menu(WINDOW* win, std::vector<std::string> options, int number_of_options, std::vector<int> modes, int option_highlighted)
: menu_window_(win), options_(options), number_of_options_(number_of_options), modes_(modes), option_highlighted_(option_highlighted) {}

int Menu::chooseOption()
{
    int input = wgetch(menu_window_);
    switch (input)
    {
        case (int)'w':
            if (option_highlighted_ > 0)
                option_highlighted_--;
            break;
        case (int)'s':
            if (option_highlighted_ < number_of_options_ - 1)
                option_highlighted_++;
            break;
        case KEY_UP:
            if (option_highlighted_ > 0)
                option_highlighted_--;
            break;
        case KEY_DOWN:
            if (option_highlighted_ < number_of_options_ - 1)
                option_highlighted_++;
            break;
        case 10: //10 is enter key for some reason 
            wclear(menu_window_);
            wrefresh(menu_window_);
            return modes_[option_highlighted_] + 1;
        default:
            break;
    }
    return 0;
}

MainMenu::MainMenu(WINDOW* win, std::vector<std::string> options, int number_of_options, std::vector<int> modes, int option_highlighted)
 : Menu(win, options, number_of_options, modes) {}

void MainMenu::displayMenu() const
{
    box(menu_window_, 0, 0);
    wmove(menu_window_, 10, 10);
    wprintw(menu_window_, "This is the main menu space for game title and all");
    for (int i = 0; i < number_of_options_; i++)
    {
        if (i == option_highlighted_)
            wattron(menu_window_, A_REVERSE); //are there terminals that don't support this attribute?
        wmove(menu_window_, 12 + 2 * i, 10);
        wprintw(menu_window_, options_[i].c_str());
        wattroff(menu_window_, A_REVERSE);
    }
}


InfoMenu::InfoMenu(WINDOW* win, std::vector<std::string> options, int number_of_options, std::vector<int> modes, int option_highlighted)
 : Menu(win, options, number_of_options, modes) {}

void InfoMenu::displayMenu() const
{
    box(menu_window_, 0, 0);
    wmove(menu_window_, 3, 3);
    wprintw(menu_window_, "This is where the information goes");
    wattron(menu_window_, A_REVERSE);
    wmove(menu_window_, 8, 3);
    wprintw(menu_window_, options_[0].c_str());
    wattroff(menu_window_, A_REVERSE);
}


PauseMenu::PauseMenu(WINDOW* win, std::vector<std::string> options, int number_of_options, std::vector<int> modes, int option_highlighted)
 : Menu(win, options, number_of_options, modes) {}

void PauseMenu::displayMenu() const
{
    box(menu_window_, 0, 0);
    wmove(menu_window_, 3, 3);
    wprintw(menu_window_, "This is where the pause menu text shows up");
    for (int i = 0; i < number_of_options_; i++)
    {
        if (i == option_highlighted_)
            wattron(menu_window_, A_REVERSE);
        wmove(menu_window_, 6, 3 + 20 * i);
        wprintw(menu_window_, options_[i].c_str());
        wattroff(menu_window_, A_REVERSE);
    }
}

int PauseMenu::chooseOption()
{
    int input = wgetch(menu_window_);
    switch (input)
    {
        case (int)'d':
            if (option_highlighted_ < number_of_options_ - 1)
                option_highlighted_++;
            break;
        case KEY_RIGHT:
            if (option_highlighted_ < number_of_options_ - 1)
                option_highlighted_++;
            break;
        case (int)'a':
            if (option_highlighted_ > 0)
                option_highlighted_--;
            break;
        case KEY_LEFT:
            if (option_highlighted_ > 0)
                option_highlighted_--;
            break;
        case 10: //10 is enter key
            wclear(menu_window_);
            wrefresh(menu_window_);
            return modes_[option_highlighted_] + 1;
        default:
            break;
    }
    return 0;
}


DeathMenu::DeathMenu(WINDOW* win, std::vector<std::string> options, int number_of_options, std::vector<int> modes, int option_highlighted)
 : PauseMenu(win, options, number_of_options, modes) {}

void DeathMenu::displayMenu() const
{
    box(menu_window_, 0, 0);
    wmove(menu_window_, 3, 3);
    wprintw(menu_window_, "This is where the death text will be");
    for (int i = 0; i < number_of_options_; i++)
    {
        if (i == option_highlighted_)
            wattron(menu_window_, A_REVERSE);
        wmove(menu_window_, 6, 3 + 20 * i);
        wprintw(menu_window_, options_[i].c_str());
        wattroff(menu_window_, A_REVERSE);
    }
}