#include "main_header.hpp"

//base class for all enemies
class Enemy : public LivingEntity {
public:
    Enemy(pair<int> pos, int speed, int aggro_distance, int direction, bool aggroed = false);
    virtual void display(WINDOW*) const override;
    void nonAggroMovement(); //gotta modify these with wall detection
    void aggroMovement(const PlayerCharacter&);
    void movement(const PlayerCharacter&);
    void detectAggro(const PlayerCharacter&, const LevelGrid&);
private:
    int aggro_distance_;
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