#include "enemies.hpp"
#include "characters.hpp"

//Enemy base class definition
Enemy::Enemy(pair<int> pos, int speed, int aggro_distance, int direction, bool aggroed) : LivingEntity(pos, direction, speed), aggro_distance_(aggro_distance), aggroed_(aggroed) {}

void Enemy::display(WINDOW* win) const
{
    wmove(win, pos_.y_, pos_.x_);
    switch(direction_)
    {
        case 0:
            waddch(win, '^');
            break;
        case 1:
            waddch(win, '>');
            break;
        case 2:
            waddch(win, 'V');
            break;
        case 3:
            waddch(win, '<');
            break;
    }
}

void Enemy::nonAggroMovement()
{
    switch (direction_)
    {
        case 0:
            pos_.y_--;
        case 1:
            pos_.x_++;
        case 2:
            pos_.y_++;
        case 3:
            pos_.x_--;
    }
}

void Enemy::aggroMovement(const PlayerCharacter& player)
{
    pair<int> distance = player.getPos() - pos_;
    if (distance.x_ > distance.y_)
    {
        //approach player on x axis
        pos_.x_ += distance.x_ / abs(distance.x_);
        return;
    }
    //approach player on y axis
    pos_.y_ += distance.y_ / abs(distance.y_);
}

void Enemy::movement(const PlayerCharacter& player)
{
    if (aggroed_)
    {
        aggroMovement(player);
        return;
    }
    nonAggroMovement();
}

void Enemy::detectAggro(const PlayerCharacter& player, const LevelGrid& level)
{
    pair<int> distance_coord = player.getPos() - pos_;
    float distance_magnitude_squared = distance_coord.x_ * distance_coord.x_ + distance_coord.y_ * distance_coord.y_;
    if (!(distance_magnitude_squared <= aggro_distance_ * aggro_distance_)) //verify that the player is in range
    {   
        aggroed_ = false;
        return;
    }
    pair<int> abs_distance_coord(abs(distance_coord.x_), abs(distance_coord.y_));
    switch (direction_) //returns if the player is not in front of the enemy
    {
        case 0:
            if (!(distance_coord.y_ < 0)) //check that player is above enemy
            {
                aggroed_ = false;
                return;
            }
            //check if y coord is bigger than x, aka if player is above the V line above the enemy
            if (!(abs_distance_coord.y_ > abs_distance_coord.x_ / 2))
            {
                aggroed_ = false;
                return;
            }
            break;
        case 1:
            if (!(distance_coord.x_ > 0))
            {
                aggroed_ = false;
                return;
            }
            if (!(distance_coord.x_ > abs_distance_coord.y_ / 2))
            {
                aggroed_ = false;
                return;
            }
            break;
        case 2:
            if (!(distance_coord.y_ > 0))
            {
                aggroed_ = false;
                return;
            }
            if (!(distance_coord.y_ > abs_distance_coord.x_ / 2))
            {
                aggroed_ = false;
                return;
            }
            break;
        case 3:
            if (!(distance_coord.x_ < 0))
            {
                aggroed_ = false;
                return;
            }
            if ((abs_distance_coord.x_ > abs_distance_coord.y_ / 2))
            {
                aggroed_ = false;
                return;
            }
            break;
    }
    //now check if a wall is in the way
    int count = 0;
    int weak_index = 0;
    int adder = 1;
    if (abs_distance_coord.x_ <= abs_distance_coord.y_) 
    {
        int number_of_y_jumps_per_x_jump = abs_distance_coord.y_ / abs_distance_coord.x_;
        if (distance_coord.x_ > 0) //checks if we want to go right or left the x axis from the player
            adder = -1;
        weak_index = player.getPos().x_;
        if (distance_coord.y_ < 0) //situation where player is above enemy
        {
            for (int i = player.getPos().y_ + 1; i < pos_.y_; i++)
            {
                if (level.level_walls_[i][weak_index])
                {
                    aggroed_ = false;
                    return;
                }
                if (count % number_of_y_jumps_per_x_jump == 0)
                    weak_index += adder;
            }
        }
        if (distance_coord.y_ > 0) //situation where player is below enemy
        {
            for (int i = player.getPos().y_ - 1; i > pos_.y_; i--)
            {
                if (level.level_walls_[i][weak_index])
                {
                    aggroed_ = false;
                    return;
                }
                if (count % number_of_y_jumps_per_x_jump == 0)
                    weak_index += adder;
            }
        }
        aggroed_ = true;
        return;
    }

    if (abs_distance_coord.y_ < abs_distance_coord.x_)
    {
        int number_of_x_jumps_per_y_jump = abs_distance_coord.x_ / abs_distance_coord.y_;
        if (distance_coord.y_ > 0) //checks if we want to go up or down the y axis from the player
                adder = -1;
        weak_index = player.getPos().y_;
        if (distance_coord.x_ < 0) //situation where the player is at the enemy's left
        {
            for (int i = player.getPos().x_ + 1; i < pos_.x_; i++)
            {
                if (level.level_walls_[weak_index][i])
                {
                    aggroed_ = false;
                    return;
                }
                if (count % number_of_x_jumps_per_y_jump == 0)
                    weak_index += adder;
            }
            aggroed_ = true;
            return;
        }
        //situation where the player is at the enemy's right
        if (distance_coord.x_ > 0)
        {
            for (int i = player.getPos().x_ - 1; i > pos_.x_; i--)
            {
                if (level.level_walls_[weak_index][i])
                {
                    aggroed_ = false;
                    return;
                }
                if (count % number_of_x_jumps_per_y_jump == 0)
                    weak_index += adder;
            }
            aggroed_ = true;
            return;
        }
    }
    aggroed_ = false;
    endwin();
    std::cout << "something went wrong with the aggro detection\n";
}