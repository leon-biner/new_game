#pragma once
#include <ncurses.h>
#include <chrono>
#include "main_header.hpp"
#include "characters.hpp"

class LivingEntity;
class LevelGrid;
class PlayerCharacter;

//base class for all enemies
class Enemy : public LivingEntity {
public:
    Enemy(mypair<int> pos, int speed, mypair<int> aggro_range_, int direction, int damage, bool aggroed = false);
    void displayAttributes();
    void showAggro(const LevelGrid&);
    virtual void display(WINDOW*) const override;
    bool readyToMove(std::chrono::time_point<std::chrono::high_resolution_clock> current_time);
    void updateWallsTouched(const LevelGrid&) override;
    void movement(const PlayerCharacter&, const LevelGrid&); //does aggroMovement() if aggroed, nonAggroMovement() if not
    bool move(); //returns false if there's a wall in the way, updates pos in the direction of direction_
    bool detectHit(const PlayerCharacter&);
    void nonAggroMovement(const LevelGrid& level); //moves in direction of direction_, if theres a wall it does a rotation of 180 deg
    void aggroMovement(const PlayerCharacter&, const LevelGrid&); //follows the character
    void detectAggro(const PlayerCharacter&, const LevelGrid&); //updates aggroed to true if player is in smaller range, or in bigger range and 
    //in front of the enemy with a curve of y = abs(x / 2) if looking up, and also looks for walls in the diagonal between player and enemy
    int getDamage() const;
    //mypair<int> getPos() const;
    void resetEnemy(std::pair<mypair<int>, int> enemy_pos_direction_);
private:
    int damage_;
    mypair<int> aggro_range_; //first element (y_) is small radius, where he is aggroed no matter the direction, and second one is radius where he is never aggroed
                            //he is aggroed in between both ranges if he looks in the direction of the player with a scope of above abs(x / 2)
    int smaller_radius_; //just because I need to keep this stable because I change aggro_range_ later, I just added this at the end
    bool aggroed_;
    std::chrono::milliseconds time_between_moves_;
    std::chrono::time_point<std::chrono::high_resolution_clock> time_since_last_move_;
    //add colour attributes
};