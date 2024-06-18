#include "main_header.hpp"

LevelGrid::LevelGrid(pair<int> level_dimensions, const std::array<std::array<int, X_LEVEL_DIMENSIONS>, Y_LEVEL_DIMENSIONS>& level_walls, pair<int> initial_palyer_pos, 
                    const std::vector<pair<int>>& initial_enemy_pos) : level_dimensions_(level_dimensions), level_walls_(level_walls), initial_player_pos_(initial_palyer_pos), initial_enemy_pos_(initial_enemy_pos) {}

void LevelGrid::displayWalls(WINDOW* win)
{
    for (int i = 0; i < level_dimensions_.y_; i++)
    {
        for (int j = 0; j < level_dimensions_.x_; j++)
        {
            wmove(win, i, j);
            switch (level_walls_[i][j])
            {
                case 0:
                    continue;
                case 1:
                    waddch(win, ACS_VLINE);
                    break;
                case 2:
                    waddch(win, ACS_HLINE);
                    break;
                case 3:
                    waddch(win, ACS_PLUS);
                    break;
                case 4:
                    waddch(win, ACS_TTEE);
                    break;
                case 5:
                    waddch(win, ACS_RTEE);
                    break;
                case 6:
                    waddch(win, ACS_BTEE);
                    break;
                case 7:
                    waddch(win, ACS_LTEE);
                    break;
                case 8:
                    waddch(win, ACS_URCORNER);
                    break;
                case 9:
                    waddch(win, ACS_LRCORNER);
                    break;
                case 10:
                    waddch(win, ACS_LLCORNER);
                    break;
                case 11:
                    waddch(win, ACS_ULCORNER);
                    break;
                default:
                    break;
            }
        }
    }
}

Entity::Entity(pair<int> pos) : pos_(pos) {}

Entity::~Entity() = default;

LivingEntity::LivingEntity(pair<int> pos, int direction, int speed) : Entity(pos), direction_(direction), speed_(speed), walls_touched_(4, false) {}

int integerDivisionRounded(int numerator, int denominator)
{
    int result_rounded_down = numerator / denominator;
    int rest = numerator % denominator;
    return (rest < denominator / 2) ? result_rounded_down : result_rounded_down + 1;
}

std::array<std::array<int, X_LEVEL_DIMENSIONS>, Y_LEVEL_DIMENSIONS> makeLevel_1()
{
    std::array<std::array<int, X_LEVEL_DIMENSIONS>, Y_LEVEL_DIMENSIONS> walls;
    for (int y = 0; y < Y_LEVEL_DIMENSIONS; y++)
    {
        for (int x = 0; x < X_LEVEL_DIMENSIONS; x++)
        {
            int& tmp = walls[y][x];
            if (x == 0 && y == 0) //corners
                tmp = 11;
            else if (x == X_LEVEL_DIMENSIONS - 1 && y == 0)
                tmp = 8;
            else if (x == X_LEVEL_DIMENSIONS - 1 && y == Y_LEVEL_DIMENSIONS - 1)
                tmp = 9;
            else if (x == 0 && y == Y_LEVEL_DIMENSIONS - 1)
                tmp = 10;
            else if (y == 0 && (x > 37 && x < 43)) //exit
                tmp = 100;
            else if ((x == 30 || x == 50) && y == 0) //three small vertical walls in the middle
                tmp = 4;
            else if ((x == 30 || x == 50) && y == Y_LEVEL_DIMENSIONS - 1)
                tmp = 6;
            else if ((x == 30 && y < 10) || (x == 30 && y < 30 && y > 20) || (x == 30 && y > 35))
                tmp = 1;
            else if ((x == 50 && y < 15) || (x == 50 && y < 30 && y > 20) || (x == 50 && y > 35))
                tmp = 1;
            else if (y == 30 && x == 30) //lower left horizontal wall
                tmp = 9;
            else if (y == 30 && x == 0)
                tmp = 7;
            else if ((y == 30 && x < 30))
                tmp = 2;
            else if (x == 30 && y == 35) //horizontal wall above two lower vertical walls
                tmp = 11;
            else if (x > 30 && x < 50 && y == 35)
                tmp = 2;
            else if (x == 50 && y == 35)
                tmp = 8;
            else if (x == 30 && y == 20) //horizontal wall above two middle vertical walls
                tmp = 11;
            else if (x > 30 && x < 50 && y == 20)
                tmp = 2;
            else if (x == 50 && y == 20)
                tmp = 8;
            else if (x == 50 && y == 30) //wall to the right of the middle walls
                tmp = 10;
            else if (x > 50 && x < 65 && y == 30)
                tmp = 2;
            else if (x == 65 && y == 30)
                tmp = 5;
            else if (x == 65 && y > 5 && y < 35)
                tmp = 1;
            else if (x == 50 && y == 15) //horizontal wall in the middle from right middle wall
                tmp = 9;
            else if (y == 15 && x < 50 && x > 15)
                tmp = 2;
            else if (y == 15 && x == 15)
                tmp = 7;
            else if (x == 15 && y > 12 && y < 18) //vertical wall attached to previous wall
                tmp = 1;
            else if (x == 0 || x == X_LEVEL_DIMENSIONS - 1) //vertical walls
                tmp = 1;
            else if (y == 0 || y == Y_LEVEL_DIMENSIONS - 1) //horizontal walls
                tmp = 2;
            else
                tmp = 0;
        }
    }
    return walls;
}