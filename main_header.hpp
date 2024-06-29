#pragma once

#include <ncurses.h>
#include <map>
#include <vector>

const int X_LEVEL_DIMENSIONS = 80;
const int Y_LEVEL_DIMENSIONS = 40;

class PlayerCharacter;
class Enemy;
class Entity;
class Button;
class HorizontalSlideDoor;

//I know there is a stl pair class but I wanted to make my own specifically for this program's purpose
template <typename T>
struct mypair {
    mypair() = default;
    mypair(T y, T x) : x_(x), y_(y) {}
    T y_;
    T x_;
    mypair<T> operator-(const mypair<T>& right_arg) const 
    {
        return mypair<T>(y_ - right_arg.y_, x_ - right_arg.x_);
    }
    bool operator==(const mypair<T>& right_arg) const
    {
        if (x_ == right_arg.x_ && y_ == right_arg.y_)
            return true;
        return false;
    }
    mypair<T>& operator=(const mypair<T>& right_arg)
    {
        if (this == &right_arg)
            return *this;
        x_ = right_arg.x_;
        y_ = right_arg.y_;
        return *this;
    }
};

class LevelGrid { //contains the walls and other non-living entities as well as enemies, 
                //and a ref to the player, anything proper to the level
public:
    LevelGrid(std::array<std::array<int, X_LEVEL_DIMENSIONS>, Y_LEVEL_DIMENSIONS> walls, mypair<int> player_pos
    , const std::vector<std::pair<mypair<int>, int>> enemy_pos, std::vector<std::shared_ptr<Enemy>>
    , std::vector<std::shared_ptr<Button>>, std::map<std::shared_ptr<Button>, std::shared_ptr<HorizontalSlideDoor>>, PlayerCharacter&);
    void displayWalls(WINDOW*) const;
    void displayEntities(WINDOW*) const;
    void updateButtonsAndDoors();
    void updateEnemies();
    void resetLevel();
    int detectHit() const; //returns damage dealt to player
    void closeDoor(const std::shared_ptr<Button>); //checks if door can be opened
    void openDoor(const std::shared_ptr<Button>); //checks if door can be closed
    const mypair<int> initial_player_pos_;
    const std::vector<std::pair<mypair<int>, int>> initial_enemy_pos_direction_; //first elem is enemy pos, second is direction
    //2D array, first index is y, where each value gives the direction of the wall: 0 if no wall, 1 if vertical, 2 if horizontal
    //3 if cross, T shapes : normal:4, right:5, down:6, left:7, corners: 8:URC, 9:LRC, 10:LLC, 11:ULC, 12 horiz. door, 100 is level end
    std::array<std::array<int, X_LEVEL_DIMENSIONS>, Y_LEVEL_DIMENSIONS> level_walls_;
private:
    //std::map<std::string, std::vector<std::shared_ptr<Entity>>>  entities_; //key is the name of the entity wanted, player not included
    ////keys : enemies, buttons, maybe make globals with these keys
    std::vector<std::shared_ptr<Enemy>> enemies_;
    std::vector<std::shared_ptr<Button>> buttons_;
    std::map<std::shared_ptr<Button>, std::shared_ptr<HorizontalSlideDoor>> doors_;
    PlayerCharacter& player_;
};

class Entity {//base class for everything added in the level
public:
    Entity() = default;
    Entity(mypair<int> pos);
    virtual ~Entity();
    virtual void display(WINDOW*) const = 0;
protected:
    mypair<int> pos_;
};

class HorizontalSlideDoor { //the position is the ULC, slides fro, left to right to open, and right to left to close
public:
    HorizontalSlideDoor() = default;
    HorizontalSlideDoor(mypair<int> pos, int number_of_doors, int number_of_doors_closed = 0);
    bool okToOpen() const; //returns true if we can open the next door pallet
    bool okToClose() const; //returns true if we can close the next door pallet
    mypair<int> posNextDoorToClose() const;
    void updateDoorsOpen(int);
private:
    mypair<int> pos_;
    int number_of_doors_closed_;
    int number_of_doors_;
};

//I need to add a color attribute to each button to differentiate them
class Button : public Entity { //button that goes on when you step on it and off when you step off of it if temporary_ or stays on
public:
    Button() = default;
    Button(mypair<int> pos, bool temp, bool closes_door, char character = '#');
    virtual void display(WINDOW*) const override;
    void updatePressedStatus(const PlayerCharacter&); //updates pressed_ to true if button is pressed;
    bool wantToCloseDoor() const;
    bool readyToMove(std::chrono::time_point<std::chrono::high_resolution_clock> current_time);
    bool operator<(const Button&) const;
    int id;
private:
    bool temporary_;
    bool pressed_; //permanent buttons stay pressed
    char character_;
    bool closes_door_; //true if the buttons opens the door, false is it closes the door
    std::chrono::milliseconds time_between_moves_;
    std::chrono::time_point<std::chrono::high_resolution_clock> time_since_last_move_;
    //add colour attribute
};

class LivingEntity : public Entity {
public:
    LivingEntity(mypair<int> pos, int direction, int speed);
    virtual void updateWallsTouched(const LevelGrid&) = 0;
protected:
    int direction_; //0 if up, 1 if right, 2 if down, 3 if left
    int speed_;
    std::vector<bool> walls_touched_; //true at the index if a wall is next to the player, same way for the indexes as direction_
};


int integerDivisionRounded(int numerator, int denominator);

std::array<std::array<int, X_LEVEL_DIMENSIONS>, Y_LEVEL_DIMENSIONS> makeLevel_1Walls();
std::array<std::array<int, X_LEVEL_DIMENSIONS>, Y_LEVEL_DIMENSIONS> makeLevel_2Walls();
std::array<std::array<int, X_LEVEL_DIMENSIONS>, Y_LEVEL_DIMENSIONS> makeLevel_3Walls();

//I do not remember what this is for at all godammit
// std::vector<bool>& detectWall(std::vector<bool>&, WINDOW*, PlayerCharacter& player, std::vector<Enemy>& enemies) //maybe use another container than a vector here, but if it stays small a vector will do just fine
// {

// }

//After, I will add colour. I will keep a global map of colour for each enemy and character with a string key (name of enemy/character) and int value (id of colour pair)