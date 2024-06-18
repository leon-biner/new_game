#pragma once

#include <ncurses.h>
#include <iostream>
#include <vector>
#include <array>

const int X_LEVEL_DIMENSIONS = 80;
const int Y_LEVEL_DIMENSIONS = 40;
//bool GAME_OVER = false;

//I know there is a stl pair class but I wanted to make my own specifically for this program's purpose
template <typename T>
struct pair {
    pair(T arg_1, T arg_2) : x_(arg_2), y_(arg_1) {}
    T y_;
    T x_;
    pair<T> operator-(const pair<T>& right_arg) const 
    {
        return pair<T>(y_ - right_arg.y_, x_ - right_arg.x_);
    }
    bool operator==(const pair<T>& right_arg) const
    {
        if (x_ == right_arg.x_ && y_ == right_arg.y_)
            return true;
        return false;
    }
    pair<T>& operator=(const pair<T>& right_arg)
    {
        if (this == &right_arg)
            return *this;
        x_ = right_arg.x_;
        y_ = right_arg.y_;
        return *this;
    }
};

class PlayerCharacter; //get rid of this shit once I learn about include guards
class Enemy;

class LevelGrid {
public:
    LevelGrid(pair<int>, const std::array<std::array<int, X_LEVEL_DIMENSIONS>, Y_LEVEL_DIMENSIONS>&, pair<int>, const std::vector<pair<int>>&);
    void displayWalls(WINDOW*);
    const pair<int> initial_player_pos_;
    const std::vector<pair<int>>& initial_enemy_pos_;
    const pair<int> level_dimensions_;
    //2D array, first index is y, where each value gives the direction of the wall: 0 if no wall, 1 if vertical, 2 if horizontal
    //3 if cross, T shapes : 4 if normal T, 5 if right, 6 if upside down, 7 if left, corners: 8:URC, 9:LRC, 10:LLC, 11:ULC, 100 is level end
    const std::array<std::array<int, X_LEVEL_DIMENSIONS>, Y_LEVEL_DIMENSIONS> level_walls_;
};

class Entity {
public:
    Entity(pair<int> pos);
    virtual ~Entity();
    virtual void display(WINDOW*) const = 0;
protected:
    pair<int> pos_;
};

class LivingEntity : public Entity {
public:
    LivingEntity(pair<int> pos, int direction, int speed);
    virtual void updateWallsTouched(const LevelGrid&) = 0;
protected:
    int direction_; //0 if up, 1 if right, 2 if down, 3 if left
    int speed_;
    std::vector<bool> walls_touched_; //true at the index if a wall is next to the player, same way for the indexes as direction_
};

int integerDivisionRounded(int numerator, int denominator);

void createLevels(std::vector<LevelGrid>&);
std::array<std::array<int, X_LEVEL_DIMENSIONS>, Y_LEVEL_DIMENSIONS> makeLevel_1();
void makeLevel_2(std::array<std::array<int, X_LEVEL_DIMENSIONS>, Y_LEVEL_DIMENSIONS>&);
void makeLevel_3(std::array<std::array<int, X_LEVEL_DIMENSIONS>, Y_LEVEL_DIMENSIONS>&);


//I do not remember what this is for at all godammit
// std::vector<bool>& detectWall(std::vector<bool>&, WINDOW*, PlayerCharacter& player, std::vector<Enemy>& enemies) //maybe use another container than a vector here, but if it stays small a vector will do just fine
// {

// }

//After, I will add colour. I will keep a global map of colour for each enemy and character with a string key (name of enemy/character) and int value (id of colour pair)