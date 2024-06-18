#include "main_header.hpp"

//base class for all enemies
class Enemy : public LivingEntity {
public:
    Enemy(pair<int> pos, int speed, pair<int> aggro_range_, int direction, int damage, bool pause = false, bool aggroed = false);
    void displayAttributes();
    void showAggro(const LevelGrid&);
    virtual void display(WINDOW*) const override;
    void updateWallsTouched(const LevelGrid&) override;
    void movement(const PlayerCharacter&, const LevelGrid&); //does aggroMovement() if aggroed, nonAggroMovement() if not
    bool move(); //returns false if there's a wall in the way, updates pos in the direction of direction_
    bool detectHit(const PlayerCharacter&) const;
    void nonAggroMovement(const LevelGrid& level); //moves in direction of direction_, if theres a wall it does a rotation of 180 deg
    void aggroMovement(const PlayerCharacter&, const LevelGrid&); //follows the character
    void detectAggro(const PlayerCharacter&, const LevelGrid&); //updates aggroed to true if player is in smaller range, or in bigger range and 
    //in front of the enemy with a curve of y = abs(x / 2) if looking up, and also looks for walls in the diagonal between player and enemy
    int getDamage() const;
    friend void resetEnemiesPos(std::vector<Enemy>& enemies, const LevelGrid& level);
private:
    bool pause_;
    int damage_;
    pair<int> aggro_range_; //first element is small radius, where he is aggroed no matter the direction, and second one is radius where he is never aggroed
                            //he is aggroed in between both ranges if he looks in the direction of the player with a scope of above abs(x / 2)
    bool aggroed_;
    
};

class EvilMan : public Enemy { //evil man is just the default enemy type
public:
    EvilMan(pair<int> pos, int speed, int aggro_distance, int direction, bool aggroed = false);
    void display(WINDOW*) const override;
};

class SpeedyMeany : public Enemy { //speedy meany is faster when he is aggroed
public:
    SpeedyMeany(pair<int> pos, int speed, int aggro_distance, int direction, bool aggroed = false);
    void display(WINDOW*) const override;
};