#include "characters.hpp"
#include "enemies.hpp"

// PlayerCharacter base class definition
PlayerCharacter::PlayerCharacter(mypair<int> pos, int speed, int health, int direction, int lives, char core_character) 
: LivingEntity(pos, direction, speed), health_(health), health_memory_(health), lives_(lives), core_character_(core_character) {}

void PlayerCharacter::displayAttributes()
{
    printw("Player :");
    mvprintw(5, X_LEVEL_DIMENSIONS + 2, "position is : (%i, %i)", pos_.x_, pos_.y_);
    mvprintw(6, X_LEVEL_DIMENSIONS + 2, "direction is : %i", direction_);
    mvprintw(7, X_LEVEL_DIMENSIONS + 2, "lives : %i", lives_);
    mvprintw(8, X_LEVEL_DIMENSIONS + 2, "health : %i", health_);
}

void PlayerCharacter::move(const LevelGrid& level, int input)
{
    switch (input)
        {
            case (int)'w':
                moveUp(level);
                break;
            case (int)'d':
                moveRight(level);
                break;
            case (int)'s':
                moveDown(level);
                break;
            case (int)'a':
                moveLeft(level);
                break;
            case KEY_UP:
                moveUp(level);
                break;
            case KEY_RIGHT:
                moveRight(level);
                break;
            case KEY_DOWN:
                moveDown(level);
                break;
            case KEY_LEFT:
                moveLeft(level);
                break;
            default:
                break;
        }
}

void PlayerCharacter::moveUp(const LevelGrid& level)
{
    direction_ = 0;
    if (level.level_walls_[pos_.y_ - 1][pos_.x_] || level.level_walls_[pos_.y_ - 2][pos_.x_])
        return; //maybe add a red colour or smth to show he hit a wall?
    // if (level.level_walls_[pos_.y_ - 2][pos_.x_])
    //     direction_ = 1;
    pos_.y_--;
}

void PlayerCharacter::moveDown(const LevelGrid& level)
{
    direction_ = 2;
    if (level.level_walls_[pos_.y_ + 1][pos_.x_] || level.level_walls_[pos_.y_ + 2][pos_.x_])
        return;
    // if (level.level_walls_[pos_.y_ + 2][pos_.x_])
    //     direction_ = 3;
    pos_.y_++;
}

void PlayerCharacter::moveLeft(const LevelGrid& level)
{
    direction_ = 3;
    if (level.level_walls_[pos_.y_][pos_.x_ - 1] || level.level_walls_[pos_.y_][pos_.x_ - 2])
        return;
    // if (level.level_walls_[pos_.y_][pos_.x_ - 2])
    //     direction_ = 0;
    pos_.x_--;
}

void PlayerCharacter::moveRight(const LevelGrid& level)
{
    direction_ = 1;
    if (level.level_walls_[pos_.y_][pos_.x_ + 1] || level.level_walls_[pos_.y_][pos_.x_ + 2])
        return;
    // if (level.level_walls_[pos_.y_][pos_.x_ + 2])
    //     direction_ = 2;
    pos_.x_++;
}

bool PlayerCharacter::detectHit(LevelGrid& level)
{
    int damage_dealt = 0;
    if ((damage_dealt = level.detectHit()) == 0)
        return false;
    if (updateHealth(-1 * damage_dealt))
    {
        health_ = health_memory_;
        lives_--;
        if (lives_ == 0)
        {
            return true;
        }
        resetPos(level);
        level.resetLevel();
    }
    return false;
}

void PlayerCharacter::resetPos(const LevelGrid& level)
{
    pos_ = level.initial_player_pos_;
}

mypair<int> PlayerCharacter::getPos() const
{
    return pos_;
}

bool PlayerCharacter::updateHealth(int health_gain)
{
    health_ += health_gain;
    return (health_ <= 0) ? true : false;
}

void PlayerCharacter::display(WINDOW* win) const
{
    switch (direction_)
    {
        case 0:
            wmove(win, pos_.y_ - 1, pos_.x_);
            waddch(win, '|');
            break;
        case 1:
            wmove(win, pos_.y_, pos_.x_);
            wprintw(win, "%c-", core_character_);
            return;
        case 2:
            wmove(win, pos_.y_ + 1, pos_.x_);
            waddch(win, '|');
            break;
        case 3:
            wmove (win, pos_.y_, pos_.x_ - 1);
            wprintw(win, "-%c", core_character_);
            return;
    }
    wmove(win, pos_.y_, pos_.x_);
    waddch(win, core_character_);
}

void PlayerCharacter::updateWallsTouched(const LevelGrid& level)
{
    switch (direction_)
    {
        case 0:
            //check for walls touched with nose in the up direction
            checkWalls(level, true, false, false, false);
            break;
        case 1:
            //check for walls touched with nose in the right direction
            checkWalls(level, false, true, false, false);
            break;
        case 2:
            //check for walls touched with nose in the down direction
            checkWalls(level, false, false, true, false);
            break;
        case 3:
            //check for walls touched with nose in the left direction
            checkWalls(level, false, false, false, true);
            break;
    }
}


void PlayerCharacter::checkWalls(const LevelGrid& level, bool up, bool right, bool down, bool left)
{
    if (level.level_walls_[pos_.y_ - 1 - up][pos_.x_])
        walls_touched_[0] = true;
    if (level.level_walls_[pos_.y_][pos_.x_ + 1 + right])
        walls_touched_[1] = true;
    if (level.level_walls_[pos_.y_ + 1 + down][pos_.x_])
        walls_touched_[2] = true;
    if (level.level_walls_[pos_.y_][pos_.x_ - 1 - left]) 
        walls_touched_[3] = true;
}

bool PlayerCharacter::goToNextLevel(const LevelGrid& level) const
{
    if (level.level_walls_[pos_.y_ - 2][pos_.x_] == 100 || level.level_walls_[pos_.y_ + 2][pos_.x_] == 100)
        return true;
    return false;
}

//int PlayerCharacter::getHealth() const{return health_;}

/*
//MisterRegular derived class definition
MisterRegular::MisterRegular(mypair<int> pos, int speed, int health, int direction, int lives, char core_character) :
               PlayerCharacter(pos, speed, health, direction, lives, core_character) {}

//MuscleMan derived class definition
MuscleMan::MuscleMan(mypair<int> pos, int speed, int health, char core_character) :
         PlayerCharacter(pos, speed, health, core_character) {}

void MuscleMan::updateHealth(int health_gain)
{
    if (health_gain < 0)
    {
        health_ -= health_gain / 2;
        return;
    }
    health_ += health_gain;
}

//ShieldBro derived class definition
ShieldBro::ShieldBro(mypair<int> pos, int speed, int health, int shield_count, char core_character, bool shield) : 
            PlayerCharacter(pos, speed, health, core_character) , shield_(shield), shield_count_(shield_count) {}

void ShieldBro::updateHealth(int health_gain)
{
    if (shield_)
    {
        if (health_gain < 0)
        {
            shield_ = false;
            return;
        }
    }
    health_ += health_gain;
}

void ShieldBro::setShield(bool status)
{
    if (status)
    {
        if (shield_count_ >= MAX_SHIELD_COUNT)
            return;
        if (shield_)
            return;
        shield_ = true;
        return;
    }
    shield_ = false;
}

//SneakyLady derived class definition
SneakyLady::SneakyLady(mypair<int> pos, int speed, int health, int sneak_count, char core_character, bool sneak_status) :
            PlayerCharacter(pos, speed, health, core_character), sneak_status_(sneak_status), sneak_count_(sneak_count) {}

void SneakyLady::updateHealth(int health_gain)
{
    if (health_gain < 0 && sneak_status_)
            return;
    health_ += health_gain;
}

void SneakyLady::setSneakStatus(bool status)
{
    if (status)
    {
        if (sneak_count_ >= MAX_STEALTH_COUNT)
            return;
        if (sneak_status_)
            return;
        sneak_status_ = true;
    }
    if (!sneak_status_)
        return;
    sneak_status_ = false;
}
*/