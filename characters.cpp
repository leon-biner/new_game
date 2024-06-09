#include "characters.hpp"

// PlayerCharacter base class definition
PlayerCharacter::PlayerCharacter(pair<int> pos, int speed, int health, int direction, char core_character) : 
                LivingEntity(pos, direction, speed), health_(health), core_character_(core_character) {}

void PlayerCharacter::moveUp()
{
    pos_.y_--;
    direction_ = 1;
}

void PlayerCharacter::moveDown()
{
    pos_.y_++;
    direction_ = 3;
}

void PlayerCharacter::moveLeft()
{
    pos_.x_--;
    direction_ = 4;
}

void PlayerCharacter::moveRight()
{
    pos_.x_++;
    direction_ = 2;
}

pair<int> PlayerCharacter::getPos() const
{
    return pos_;
}

void PlayerCharacter::updateHealth(int health_gain)
{
    health_ += health_gain;
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

// void PlayerCharacter::checkWallDownDirection(const LevelGrid& level)
// {
//     if (level.level_walls_[pos_.y_ - 1][pos_.x_])
//         walls_touched_[0] = true;
//     if (level.level_walls_[pos_.y_][pos_.x_ + 1])
//         walls_touched_[1] = true;
//     if (level.level_walls_[pos_.y_ + 2][pos_.x_])
//         walls_touched_[2] = true;
//     if (level.level_walls_[pos_.y_][pos_.x_ - 1]) 
//         walls_touched_[3] = true;
// }

// void PlayerCharacter::checkWallRightDirection(const LevelGrid& level)
// {
//     if (level.level_walls_[pos_.y_ - 1][pos_.x_])
//         walls_touched_[0] = true;
//     if (level.level_walls_[pos_.y_][pos_.x_ + 2])
//         walls_touched_[1] = true;
//     if (level.level_walls_[pos_.y_ + 1][pos_.x_])
//         walls_touched_[2] = true;
//     if (level.level_walls_[pos_.y_][pos_.x_ - 1]) 
//         walls_touched_[3] = true;
// }

// void PlayerCharacter::checkWallLeftDirection(const LevelGrid& level)
// {
//     if (level.level_walls_[pos_.y_ - 1][pos_.x_])
//         walls_touched_[0] = true;
//     if (level.level_walls_[pos_.y_][pos_.x_ + 1])
//         walls_touched_[1] = true;
//     if (level.level_walls_[pos_.y_ + 1][pos_.x_])
//         walls_touched_[2] = true;
//     if (level.level_walls_[pos_.y_][pos_.x_ - 2]) 
//         walls_touched_[3] = true;
// }

/*

//MisterRegular derived class definition
MisterRegular::MisterRegular(pair<int> pos, int speed, int health, char core_character) :
             PlayerCharacter(pos, speed, health, core_character) {}

//MuscleMan derived class definition
MuscleMan::MuscleMan(pair<int> pos, int speed, int health, char core_character) :
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
ShieldBro::ShieldBro(pair<int> pos, int speed, int health, int shield_count, char core_character, bool shield) : 
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
SneakyLady::SneakyLady(pair<int> pos, int speed, int health, int sneak_count, char core_character, bool sneak_status) :
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