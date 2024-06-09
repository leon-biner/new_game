#pragma once

#include <ncurses.h>
#include <iostream>
#include <vector>

template <typename T>
struct pair {
    pair(T arg_1, T arg_2) : x_(arg_1), y_(arg_2) {}
    T x_;
    T y_;
    pair<T> operator-(const pair& right_arg) const 
    {
        return pair<T>(this->x_ - right_arg.x_, this->y_ - right_arg.y_);
    }
};

class PlayerCharacter; //forward declaration to friend the class
class Enemy; 

class LevelGrid {
public:
    LevelGrid(pair<int>, std::vector<std::vector<int>>);
    friend class PlayerCharacter;
    friend class Enemy;
private:
    pair<int> level_dimensions_;
    std::vector<std::vector<int>> level_walls_; //2D array, first index is y, where each value gives the direction of the wall: 0 if no wall, 1 if up, 2 if right, 3 if down, 4 if left
    //start this at -1 and end this at level_dimentions + 1 with true because I am checking player_pos + 2 to see if there's a wall, if ever it doesn't work right
};

class Entity {
public:
    //virtual destructor or smt?
    Entity(pair<int> pos);
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

//I do not remember what this is for at all godammit
// std::vector<bool>& detectWall(std::vector<bool>&, WINDOW*, PlayerCharacter& player, std::vector<Enemy>& enemies) //maybe use another container than a vector here, but if it stays small a vector will do just fine
// {

// }

//After, I will add colour. I will keep a global map of colour for each enemy and character with a string key (name of enemy/character) and int value (id of colour pair)