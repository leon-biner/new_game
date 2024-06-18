#include "enemies.hpp"
#include "characters.hpp"

//Enemy base class definition
Enemy::Enemy(pair<int> pos, int speed, pair<int> aggro_range, int direction, int damage, bool pause, bool aggroed) :
 LivingEntity(pos, direction, speed), aggro_range_(aggro_range), aggroed_(aggroed), pause_(pause), damage_(damage) {}

void Enemy::displayAttributes()
{
    printw("Enemy :");
    mvprintw(1, X_LEVEL_DIMENSIONS + 2, "position is : (%i, %i)", pos_.x_, pos_.y_);
    mvprintw(2, X_LEVEL_DIMENSIONS + 2, "direction is : %i", direction_);
    mvprintw(3, X_LEVEL_DIMENSIONS + 2, "aggroed ? : %i", aggroed_);
    // for (int i = 0; i < 4; i++)
    // {
    //     bool wall_touched = static_cast<bool>(walls_touched_[i]);
    //     mvprintw(2 + i, 21, "touches wall on direction %i ? : %i", i, wall_touched);
    // }
}

void Enemy::showAggro(const LevelGrid& level)
{
    for (int i = 0; i < Y_LEVEL_DIMENSIONS; i++)
    {
        for (int j = 0; j < X_LEVEL_DIMENSIONS; j++)
        {
            PlayerCharacter player(pair<int>(i, j), 1, 1, 1, 1, 'o');
            detectAggro(player, level);
            if (aggroed_)
            {
                wmove(stdscr, i, j);
                addch('.');
            }
        }
    }
}

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
void Enemy::updateWallsTouched(const LevelGrid& level)
{
    if (level.level_walls_[pos_.y_ - 1][pos_.x_])
        walls_touched_[0] = true;
    else
        walls_touched_[0] = false;
    if (level.level_walls_[pos_.y_][pos_.x_ + 1])
        walls_touched_[1] = true;
    else
        walls_touched_[1] = false;
    if (level.level_walls_[pos_.y_ + 1][pos_.x_])
        walls_touched_[2] = true;
    else
        walls_touched_[2] = false;
    if (level.level_walls_[pos_.y_][pos_.x_ - 1])
        walls_touched_[3] = true;
    else
        walls_touched_[3] = false;
}

void Enemy::movement(const PlayerCharacter& player, const LevelGrid& level)
{
    if (aggroed_)
        aggroMovement(player, level);
    else
        nonAggroMovement(level);
}

bool Enemy::move()
{
    if (walls_touched_[direction_])
        return false;
    switch (direction_)
    {
        case 0:
            pos_.y_--;
            break;
        case 1:
            pos_.x_++;
            break;
        case 2:
            pos_.y_++;
            break;
        case 3:
            pos_.x_--;
            break;
    }
    return true;
}

bool Enemy::detectHit(const PlayerCharacter& player) const
{
    if (player.getPos() == pos_)
        return true;
    return false;
}

void Enemy::nonAggroMovement(const LevelGrid& level)
{
    updateWallsTouched(level);
    if (!move())
    {
        direction_ += 2;
        direction_ = direction_ % 4;
    }
}

void Enemy::aggroMovement(const PlayerCharacter& player, const LevelGrid& level)
{
    updateWallsTouched(level);
    pair<int> distance = player.getPos() - pos_;
    if (abs(distance.x_) > abs(distance.y_))
    {
        direction_ = (distance.x_ > 0) ? 1 : 3;
        if (!move())
        {
            direction_ = (distance.y_ < 0) ? 0 : 2;
            move();
        }
        return;
    }
    direction_ = (distance.y_ < 0) ? 0 : 2;
    if (!move())
    {
        direction_ = (distance.x_ > 0) ? 1 : 3;
        move();
    }
}

void Enemy::detectAggro(const PlayerCharacter& player, const LevelGrid& level)
{
    pair<int> distance_coord = player.getPos() - pos_;
    float distance_magnitude_squared = distance_coord.x_ * distance_coord.x_ + distance_coord.y_ * distance_coord.y_;
    if (!(distance_magnitude_squared <= aggro_range_.x_ * aggro_range_.x_)) //verify that the player is in range, remember 2nd elem of aggro_range_ (x_) is bigger radius
    {                                                                       //and first is small radius (I know it doesn't make sense with x and y notation)
        aggroed_ = false;
        return;
    }
    if (distance_magnitude_squared <= aggro_range_.y_ * aggro_range_.y_)
    {
        aggroed_ = true;
        return;
    }
    pair<int> abs_distance_coord(abs(distance_coord.y_), abs(distance_coord.x_));
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
            if (!(abs_distance_coord.x_ > abs_distance_coord.y_ / 2))
            {
                aggroed_ = false;
                return;
            }
            break;
    }
    //now check if a wall is in the way
    int weak_index;
    int adder = 1;
    if (abs_distance_coord.x_ <= abs_distance_coord.y_) 
    {
        int number_of_y_jumps_per_x_jump = integerDivisionRounded(abs_distance_coord.y_, abs_distance_coord.x_);
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
                if (weak_index == pos_.x_ + adder)
                    continue; 
                if (i % number_of_y_jumps_per_x_jump == 0)
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
                if (weak_index == pos_.x_ + adder)
                    continue; 
                if (i % number_of_y_jumps_per_x_jump == 0)
                    weak_index += adder;
            }
        }
        aggroed_ = true;
        return;
    }

    if (abs_distance_coord.y_ < abs_distance_coord.x_)
    {
        int number_of_x_jumps_per_y_jump = integerDivisionRounded(abs_distance_coord.x_, abs_distance_coord.y_);
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
                if (weak_index == pos_.y_ + adder)
                    continue; 
                if (i % number_of_x_jumps_per_y_jump == 0)
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
                if (weak_index == pos_.y_ + adder)
                    continue; 
                if (i % number_of_x_jumps_per_y_jump == 0)
                    weak_index += adder;
            }
            aggroed_ = true;
            return;
        }
    }
}

int Enemy::getDamage() const
{
    return damage_;
}

void resetEnemiesPos(std::vector<Enemy>& enemies, const LevelGrid& level)
{
    int i = 0;
    for (auto& enemy : enemies)
    {
        enemy.pos_ = level.initial_enemy_pos_[i];
        i++;
    }
}
