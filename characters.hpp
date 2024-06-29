#pragma once
#include <ncurses.h>
#include "main_header.hpp"

class LevelGrid;
class LivingEntity;

//base class for all player characters
class PlayerCharacter : public LivingEntity {
public:
    PlayerCharacter(mypair<int> pos, int speed, int health, int direction, int lives, char core_character);
    void displayAttributes();
    void display(WINDOW*) const override;
    void move(const LevelGrid&, int input); //checks the input and uses the 4 move functions below accordingly or pauses the game
    //returns true if the player pressed pause
    void moveUp(const LevelGrid&); //updates the direction and updates the position if there is no wall in the way
    void moveDown(const LevelGrid&); //''
    void moveLeft(const LevelGrid&); //''
    void moveRight(const LevelGrid&); //''
    bool detectHit(LevelGrid& level); //looks through every enemy to see if one of them has hit the player,
    // updates health/lives, returns true if game over, resets position
    void resetPos(const LevelGrid&);
    mypair<int> getPos() const;
    virtual bool updateHealth(int); //returns true if the player's dead;
    void updateWallsTouched(const LevelGrid&) override; //I think this is useless actually we'll see
    void checkWalls(const LevelGrid&, bool, bool, bool, bool);
    bool goToNextLevel(const LevelGrid&) const;
    //int getHealth() const;
protected:
    int lives_;
    int health_;
    int health_memory_; //this sucks but the game is almost done so whatever
    char core_character_;
    //add colour attribute
};

//character 1
//mister regular is just the normal character
class MisterRegular : public PlayerCharacter {
public:
    MisterRegular(mypair<int> pos, int speed, int health, int direction, int lives, char core_character = 'O');
};

//character 2
//muscle man is slower but he has more health 
class MuscleMan : public PlayerCharacter {
public:
    MuscleMan(mypair<int> pos, int speed, int health, int direction, int lives, char core_character = '8');
    bool updateHealth(int) override;
};

//character 3
//shield bro has a shield that he can turn on for a limited time
class ShieldBro : public PlayerCharacter {
public:
    ShieldBro(mypair<int> pos, int speed, int health, int direction, int lives, 
              int shield_count, char core_character = 'H', bool shield = false);
    bool updateHealth(int) override;
    void setShield(bool); //checks internally if the player already used all his/her shields and changes colour attribute
private:
    bool shield_;
    int shield_count_;
    const int MAX_SHIELD_COUNT;
    //shield colour attribute (prob light blue)
};

//character 4
//sneaky lady can be undetectable state that she can turn on for a limited time
class SneakyLady : public PlayerCharacter {
public:
    SneakyLady(mypair<int> pos, int speed, int health, int sneak_count, char core_character = 'S', bool sneak_status = false);
    bool updateHealth(int) override;
    void setSneakStatus(bool); //checks internally if the player has used all his sneak powerups
private:
    bool sneak_status_;
    int sneak_count_;
    const int MAX_SNEAK_COUNT;
    //sneak colour attribute
};