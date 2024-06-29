#include "main_header.hpp"
#include "enemies.hpp"

//LevelGrid class definition
LevelGrid::LevelGrid(std::array<std::array<int, X_LEVEL_DIMENSIONS>, Y_LEVEL_DIMENSIONS> walls,
const mypair<int> player_pos, std::vector<std::pair<mypair<int>, int>> enemy_pos, std::vector<std::shared_ptr<Enemy>> enemies, 
std::vector<std::shared_ptr<Button>> buttons, std::map<std::shared_ptr<Button>, std::shared_ptr<HorizontalSlideDoor>> doors
, PlayerCharacter& player) :
    level_walls_(walls), initial_player_pos_(player_pos), initial_enemy_pos_direction_(enemy_pos),
    enemies_(enemies), buttons_(buttons), doors_(doors), player_(player) {}

void LevelGrid::displayWalls(WINDOW* win) const
{
    for (int i = 0; i < Y_LEVEL_DIMENSIONS; i++)
    {
        for (int j = 0; j < X_LEVEL_DIMENSIONS; j++)
        {
            wmove(win, i, j);
            switch (level_walls_[i][j])
            {
                case 0:
                    waddch(win, ' ');
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
                case 12:
                    waddch(win, '-');
                    break;
                case 100:
                    waddch(win, ' ');
                    break;
                default:
                    break;
            }
        }
    }
}

void LevelGrid::displayEntities(WINDOW* win) const
{
    for (const auto enemy : enemies_)
    {
        enemy->display(win);
    }
    for (const auto& button : buttons_)
    {
        button->display(win);
    }
}

void LevelGrid::updateButtonsAndDoors()
{
    auto current_time = std::chrono::high_resolution_clock::now();
    for (auto& button : buttons_)
    {
        if (!button->readyToMove(current_time))
            continue;
        button->updatePressedStatus(player_);
        if (button->wantToCloseDoor())
        {
            closeDoor(button);
            continue;
        }
        openDoor(button);
    }
}

void LevelGrid::updateEnemies()
{
    auto current_time = std::chrono::high_resolution_clock::now();
    for (auto& enemy : enemies_)
    {
        if (enemy->readyToMove(current_time))
            enemy->movement(player_, *this);
    }
}

void LevelGrid::resetLevel()
{
    for (int i = 0; i < enemies_.size(); i++)
    {
        enemies_[i]->resetEnemy(initial_enemy_pos_direction_[i]);
    }
}

int LevelGrid::detectHit() const
{
    for (auto& enemy : enemies_)
    {
        if (enemy->detectHit(player_))
            return enemy->getDamage();
    }
    return 0;
}

void LevelGrid::closeDoor(const std::shared_ptr<Button> button)
{
    if (doors_[button]->okToClose())
    {
        level_walls_[doors_[button]->posNextDoorToClose().y_][doors_[button]->posNextDoorToClose().x_] = 12;
        doors_[button]->updateDoorsOpen(1);
    }
}

void LevelGrid::openDoor(const std::shared_ptr<Button> button)
{
    if (doors_[button]->okToOpen())
    {
        level_walls_[doors_[button]->posNextDoorToClose().y_][doors_[button]->posNextDoorToClose().x_ - 1] = 0;
        doors_[button]->updateDoorsOpen(-1);
    }
}

//Entity class definition
Entity::Entity(mypair<int> pos) : pos_(pos) {}

Entity::~Entity() = default;


//HorizontalSlideDoor class definition
HorizontalSlideDoor::HorizontalSlideDoor(mypair<int> pos, int number_of_doors, int number_of_doors_closed)
 : pos_(pos), number_of_doors_(number_of_doors), number_of_doors_closed_(number_of_doors_closed) {}

bool HorizontalSlideDoor::okToClose() const
{
    if (number_of_doors_closed_ <= number_of_doors_)
        return true;
    return false;
}

bool HorizontalSlideDoor::okToOpen() const
{
    if (number_of_doors_closed_ > 0)
        return true;
    return false;
}

mypair<int> HorizontalSlideDoor::posNextDoorToClose() const
{
    return mypair<int>(pos_.y_, pos_.x_ + number_of_doors_closed_);
}

void HorizontalSlideDoor::updateDoorsOpen(int tmp)
{
    number_of_doors_closed_ += tmp;
    // move(0, X_LEVEL_DIMENSIONS + 2);
}

//LivingEntity class definition
LivingEntity::LivingEntity(mypair<int> pos, int direction, int speed) : Entity(pos), direction_(direction), speed_(speed), walls_touched_(4, false) {}

//Button class definition
Button::Button(mypair<int> pos, bool temp, bool closes_door, char character) 
: Entity(pos), temporary_(temp), character_(character), closes_door_(closes_door)
, time_between_moves_(std::chrono::milliseconds(500)), time_since_last_move_(std::chrono::high_resolution_clock::now()) {}

void Button::display(WINDOW* win) const
{
    if (pressed_)
        return;
    wmove(win, pos_.y_, pos_.x_);
    waddch(win, character_);
}

void Button::updatePressedStatus(const PlayerCharacter& player)
{
    if (pos_ == player.getPos())
    {
        pressed_ = true;
        return;
    }
    if (temporary_)
        pressed_ = false;
}

bool Button::wantToCloseDoor() const
{
    // check if the button is pressed or not and if it should open or close a door
    //if its not pressed, the door should !open/close
    if (pressed_)
    {
        if (closes_door_)
            return true;
        return false;
    }
    if (closes_door_)
        return false;
    return true;
}

bool Button::readyToMove(std::chrono::time_point<std::chrono::high_resolution_clock> current_time)
{
    if (std::chrono::duration_cast<std::chrono::milliseconds>(current_time - time_since_last_move_) >= time_between_moves_)
        return true;
    return false;
}

bool Button::operator<(const Button& other_button) const
{
    return other_button.id < id;
}

//Functions definitions
int integerDivisionRounded(int numerator, int denominator)
{
    int result_rounded_down = numerator / denominator;
    int rest = numerator % denominator;
    return (rest < denominator / 2) ? result_rounded_down : result_rounded_down + 1;
}

std::array<std::array<int, X_LEVEL_DIMENSIONS>, Y_LEVEL_DIMENSIONS> makeLevel_1Walls()
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
            else if (y == 0 && x == 37)
                tmp = 8;
            else if (y == 0 && x == 43)
                tmp = 11;
            else if ((x == 37 || x == 43) && y < 3)
                tmp = 1;
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
            else if (x == 65 && y > 10 && y < 35)
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

std::array<std::array<int, X_LEVEL_DIMENSIONS>, Y_LEVEL_DIMENSIONS> makeLevel_2Walls()
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
            else if (x > 37 && x < 43 && y == Y_LEVEL_DIMENSIONS - 1) //entrance from level 1
                tmp = 0;
            else if (y == Y_LEVEL_DIMENSIONS - 1 && (x > 62 && x < 68)) //exit
                tmp = 100;
            else if (y == Y_LEVEL_DIMENSIONS - 1 && x == 62)
                tmp = 9;
            else if (y == Y_LEVEL_DIMENSIONS - 1 && x == 68)
                tmp = 10;
            else if ((x == 62 || x == 68) && y > Y_LEVEL_DIMENSIONS - 4)
                tmp = 1;
            else if (x == 48 && y == Y_LEVEL_DIMENSIONS - 1) //wall to the right of the entrance
                tmp = 6;
            else if (x == 48 && y > 6 && y < Y_LEVEL_DIMENSIONS - 1)
                tmp = 1;
            else if (x == 48 && y == 33)
                tmp = 5;
            else if (y == 33 && x < 48 && x > 10) //wall just above entrance
                tmp = 2;
            else if (x == 34 && y == 0) //three walls above entrance
                tmp = 4;
            else if (x == 34 && (y < 9 || (y > 14 && y < 20) || (y > 25 && y < 31)))
                tmp = 1;
            else if (x == 65 && ((y < 9 && y > 3) || (y > 14 && y < 20) || (y > 25 && y < 31))) //vertical wall above exit
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

std::array<std::array<int, X_LEVEL_DIMENSIONS>, Y_LEVEL_DIMENSIONS> makeLevel_3Walls()
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
            else if (y == 0 && (x > 62 && x < 68)) //entrance from level 2
                tmp = 0;
            else if (y == Y_LEVEL_DIMENSIONS - 1 && (x > 37 && x < 43)) //exit
                tmp = 100;
            else if (y == Y_LEVEL_DIMENSIONS - 1 && x == 37)
                tmp = 9;
            else if (y == Y_LEVEL_DIMENSIONS - 1 && x == 43)
                tmp = 10;
            else if ((x == 37 || x == 43) && y > Y_LEVEL_DIMENSIONS - 4)
                tmp = 1;
            else if (x == 35 && y == 8) //first little hiding spot from the top
                tmp = 6;
            else if ((x == 45 || x == 35) && y < 8 && y > 5)
                tmp = 1;
            else if (x == 45 && y == 8) //cross to join walls between first and second hiding spots
                tmp = 3;
            // else if ((x > 34 && x < 46 && y == 5))
            //     tmp = 12;
            else if ((x == 45 || x == 55) && y == 8) //second little hiding spot from the top
                tmp = 4;
            else if ((x == 45 || x == 55) && y > 8 && y < 11)
                tmp = 1;
            else if ((x == 55 || x == 65) && y == 24) //third little hiding spot from the top
                tmp = 6;
            else if ((x == 55 || x == 65) && y < 24 && y > 21)
                tmp = 1;
            else if (y == 8 && x < X_LEVEL_DIMENSIONS - 1 && x > 20) //wall just below entrance
                tmp = 2;
            else if (y == 8 && x == X_LEVEL_DIMENSIONS - 1)
                tmp = 5;
            else if (y == 16 && x == 0) //2nd horizontal wall from the top
                tmp = 7;
            else if (y == 16 && x > 0 && x < 60)
                tmp = 2;
            else if (y == 24 && x == 60) //joint between 3re horizontal wall and vertical wall on the BLC
                tmp = 4;
            else if (y == 24 && x > 20 && x < X_LEVEL_DIMENSIONS - 5) //3rd horizontal wall from the top
                tmp = 2;
            else if (y == 24 && x == X_LEVEL_DIMENSIONS - 5)
                tmp = 8;
            else if (x == 60 && y == Y_LEVEL_DIMENSIONS - 1) //vertical wall on the BRC
                tmp = 6;
            else if (x == 60 && y > 24 && y < Y_LEVEL_DIMENSIONS - 1)
                tmp = 1;
            else if (y == 24 && x == 20) //vertical wall on the BLC
                tmp = 11;
            else if (x == 20 && y > 24 && y < Y_LEVEL_DIMENSIONS - 4)
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