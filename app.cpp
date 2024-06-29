#include "app.hpp"
#include "string"

Application::Application(Game game, std::map<std::string, std::shared_ptr<Menu>> menus, int mode)
: game_(game), menus_(menus), mode_(mode) {}

void Application::runApp()
{
    bool quit = false;
    while (true)
    {
        switch (mode_)
        {
            case 0:
                game_.clearWindow();
                navigateMenu("main");
                break;
            case 1:
                navigateMenu("info");
                break;
            case 2:
                playGame();
                break;
            case 3:
                navigateMenu("pause");
                break;
            case 4:
                navigateMenu("death");
                break;
            case 5:
                quit = true;
                save();
                break;
        }
        if (quit)
        {
            curs_set(1);
            endwin();
            break;
        }
    }
}

void Application::navigateMenu(std::string menu_key)
{
    do
    {
        menus_[menu_key]->displayMenu();
    } while ((mode_ = menus_[menu_key]->chooseOption()) == 0);
    mode_ = mode_ - 1;
}

void Application::playGame()
{
    int next_mode = 2;
    game_.initialisePlayer();
    while ((next_mode = game_.renderGame()) == 2)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        clear();
        game_.updateGame();
    }
    mode_ = next_mode;
}

void Application::save() {}