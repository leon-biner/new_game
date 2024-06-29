#include "app.hpp"
#include "menu.hpp"
#include "characters.hpp"
#include "enemies.hpp"
#include "main_header.hpp"
#include "game.hpp"
#include <ncurses.h>

int main()
{
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    //create the character
    PlayerCharacter player(mypair<int>(0, 0), 1, 100, 0, 5, 'O');
    //make the walls
    std::array<std::array<int, X_LEVEL_DIMENSIONS>, Y_LEVEL_DIMENSIONS> lvl_1_walls = makeLevel_1Walls();
    std::array<std::array<int, X_LEVEL_DIMENSIONS>, Y_LEVEL_DIMENSIONS> lvl_2_walls = makeLevel_2Walls();
    std::array<std::array<int, X_LEVEL_DIMENSIONS>, Y_LEVEL_DIMENSIONS> lvl_3_walls = makeLevel_3Walls();
    //make the base player pos and enemy pos and direction
    const mypair<int> lvl_1_player_pos(2, 4);
    const mypair<int> lvl_2_player_pos(2, 40);
    const mypair<int> lvl_3_player_pos(30, 2);
    std::vector<std::pair<mypair<int>, int>> lvl_1_enemy_pos_dir;
    std::vector<std::pair<mypair<int>, int>> lvl_2_enemy_pos_dir;
    std::vector<std::pair<mypair<int>, int>> lvl_3_enemy_pos_dir;
    lvl_1_enemy_pos_dir.push_back(std::pair<mypair<int>, int>(mypair<int>(10, 10), 0));
    lvl_1_enemy_pos_dir.push_back(std::pair<mypair<int>, int>(mypair<int>(20, 20), 0));
    lvl_1_enemy_pos_dir.push_back(std::pair<mypair<int>, int>(mypair<int>(30, 35), 0));
    lvl_2_enemy_pos_dir.push_back(std::pair<mypair<int>, int>(mypair<int>(10, 10), 0));
    lvl_2_enemy_pos_dir.push_back(std::pair<mypair<int>, int>(mypair<int>(20, 20), 0));
    lvl_2_enemy_pos_dir.push_back(std::pair<mypair<int>, int>(mypair<int>(30, 30), 0));
    lvl_3_enemy_pos_dir.push_back(std::pair<mypair<int>, int>(mypair<int>(10, 10), 0));
    lvl_3_enemy_pos_dir.push_back(std::pair<mypair<int>, int>(mypair<int>(20, 20), 0));
    lvl_3_enemy_pos_dir.push_back(std::pair<mypair<int>, int>(mypair<int>(30, 30), 0));
    //make the enemies
    std::vector<std::shared_ptr<Enemy>> lvl_1_enemies;
    std::vector<std::shared_ptr<Enemy>> lvl_2_enemies;
    std::vector<std::shared_ptr<Enemy>> lvl_3_enemies;
    lvl_1_enemies.push_back(std::make_shared<Enemy>(Enemy(lvl_1_enemy_pos_dir[0].first, 1, mypair<int>(3, 8), lvl_1_enemy_pos_dir[0].second, 30)));
    lvl_1_enemies.push_back(std::make_shared<Enemy>(Enemy(lvl_1_enemy_pos_dir[1].first, 1, mypair<int>(3, 8), lvl_1_enemy_pos_dir[1].second, 30)));
    lvl_1_enemies.push_back(std::make_shared<Enemy>(Enemy(lvl_1_enemy_pos_dir[2].first, 1, mypair<int>(3, 8), lvl_1_enemy_pos_dir[2].second, 30)));
    lvl_2_enemies.push_back(std::make_shared<Enemy>(Enemy(lvl_2_enemy_pos_dir[0].first, 1, mypair<int>(3, 8), lvl_2_enemy_pos_dir[0].second, 30)));
    lvl_2_enemies.push_back(std::make_shared<Enemy>(Enemy(lvl_2_enemy_pos_dir[1].first, 1, mypair<int>(3, 8), lvl_2_enemy_pos_dir[1].second, 30)));
    lvl_2_enemies.push_back(std::make_shared<Enemy>(Enemy(lvl_2_enemy_pos_dir[2].first, 1, mypair<int>(3, 8), lvl_2_enemy_pos_dir[2].second, 30)));
    lvl_3_enemies.push_back(std::make_shared<Enemy>(Enemy(lvl_3_enemy_pos_dir[0].first, 1, mypair<int>(3, 8), lvl_3_enemy_pos_dir[0].second, 30)));
    lvl_3_enemies.push_back(std::make_shared<Enemy>(Enemy(lvl_3_enemy_pos_dir[1].first, 1, mypair<int>(3, 8), lvl_3_enemy_pos_dir[1].second, 30)));
    lvl_3_enemies.push_back(std::make_shared<Enemy>(Enemy(lvl_3_enemy_pos_dir[2].first, 1, mypair<int>(3, 8), lvl_3_enemy_pos_dir[2].second, 30)));
    //make the buttons and doors
    std::vector<std::shared_ptr<Button>> lvl_3_buttons;
    std::map<std::shared_ptr<Button>, std::shared_ptr<HorizontalSlideDoor>> lvl_3_doors;
    std::shared_ptr<Button> button_1 = std::make_shared<Button>(mypair<int>(6, 40), true, true);
    std::shared_ptr<Button> button_2 = std::make_shared<Button>(mypair<int>(10, 50), true, true);
    std::shared_ptr<Button> button_3 = std::make_shared<Button>(mypair<int>(37, 62), false, false);
    std::shared_ptr<Button> button_4 = std::make_shared<Button>(mypair<int>(22, 60), true, true);
    std::shared_ptr<HorizontalSlideDoor> door_1 = std::make_shared<HorizontalSlideDoor>(mypair<int>(5, 35), 10);
    std::shared_ptr<HorizontalSlideDoor> door_2 = std::make_shared<HorizontalSlideDoor>(mypair<int>(11, 45), 10);
    std::shared_ptr<HorizontalSlideDoor> door_3 = std::make_shared<HorizontalSlideDoor>(mypair<int>(36, 37), 6);
    std::shared_ptr<HorizontalSlideDoor> door_4 = std::make_shared<HorizontalSlideDoor>(mypair<int>(21, 55), 10);
    lvl_3_buttons.push_back(button_1);
    lvl_3_buttons.push_back(button_2);
    lvl_3_buttons.push_back(button_3);
    lvl_3_buttons.push_back(button_4);
    lvl_3_doors[button_1] = door_1;
    lvl_3_doors[button_2] = door_2;
    lvl_3_doors[button_3] = door_3;
    lvl_3_doors[button_4] = door_4;
    //create the levels
    std::shared_ptr<LevelGrid> lvl_1 = std::make_shared<LevelGrid>(LevelGrid(lvl_1_walls, lvl_1_player_pos, lvl_1_enemy_pos_dir, lvl_1_enemies
    , std::vector<std::shared_ptr<Button>>(0), std::map<std::shared_ptr<Button>, std::shared_ptr<HorizontalSlideDoor>>(), player));
    std::shared_ptr<LevelGrid> lvl_2 = std::make_shared<LevelGrid>(LevelGrid(LevelGrid(lvl_2_walls, lvl_2_player_pos, lvl_2_enemy_pos_dir, lvl_2_enemies
    , std::vector<std::shared_ptr<Button>>(0), std::map<std::shared_ptr<Button>, std::shared_ptr<HorizontalSlideDoor>>(), player)));
    std::shared_ptr<LevelGrid> lvl_3 = std::make_shared<LevelGrid>((LevelGrid(lvl_3_walls, lvl_3_player_pos, lvl_3_enemy_pos_dir, lvl_3_enemies
    , lvl_3_buttons, lvl_3_doors, player)));
    std::vector<std::shared_ptr<LevelGrid>> levels = {lvl_1, lvl_2, lvl_3};
    //make the game window
    WINDOW* game_window = newwin(Y_LEVEL_DIMENSIONS, X_LEVEL_DIMENSIONS, 5, 10);
    keypad(game_window, true);
    nodelay(game_window, true);
    //construct the game with the levels and the character
    Game game(levels, player, game_window);
    //make the menu windows
    WINDOW* main_menu_window = newwin(20, 80, 5, 10);
    WINDOW* pause_menu_window = newwin(10, 80, Y_LEVEL_DIMENSIONS / 2, 10);
    WINDOW* info_menu_window = newwin(10, 80, 10, 10);
    WINDOW* death_menu_window = newwin(10, 80, Y_LEVEL_DIMENSIONS / 2, 10);
    keypad(main_menu_window, true);
    keypad(info_menu_window, true);
    keypad(pause_menu_window, true);
    keypad(death_menu_window, true);
    //make the menu options
    std::vector<std::string> main_menu_options = {"Play Game", "Info", "Save and Quit"};
    std::vector<std::string> info_menu_options = {"Back"};
    std::vector<std::string> pause_menu_options = {"Continue Playing", "Main Menu", "Save and Quit"};
    std::vector<std::string> death_menu_options = {"Main Menu", "Save and Quit"};
    //link the options to the modes of the app
    std::vector<int> main_menu_modes = {2, 1, 5};
    std::vector<int> info_menu_modes = {0};
    std::vector<int> pause_menu_modes = {2, 0, 5};
    std::vector<int> death_menu_modes = {0, 5};
    //construct the menus
    std::shared_ptr<MainMenu> main_menu = std::make_shared<MainMenu>(MainMenu(main_menu_window, main_menu_options, main_menu_options.size(), main_menu_modes));
    std::shared_ptr<PauseMenu> pause_menu = std::make_shared<PauseMenu>(PauseMenu(pause_menu_window, pause_menu_options, pause_menu_options.size(), pause_menu_modes));
    std::shared_ptr<InfoMenu> info_menu = std::make_shared<InfoMenu>(InfoMenu(info_menu_window, info_menu_options, info_menu_options.size(), info_menu_modes));
    std::shared_ptr<DeathMenu> death_menu = std::make_shared<DeathMenu>(DeathMenu(death_menu_window, death_menu_options, death_menu_options.size(), death_menu_modes));
    std::map<std::string, std::shared_ptr<Menu>> menus;
    menus["main"] = main_menu;
    menus["info"] = info_menu;
    menus["pause"] = pause_menu;
    menus["death"] = death_menu;
    //create the app
    Application app(game, menus);
    //run the app
    app.runApp();
    endwin();
}