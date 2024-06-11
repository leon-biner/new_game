#include "main_header.hpp"

//base class for all player characters
class PlayerCharacter : public LivingEntity {
public:
    PlayerCharacter(pair<int> pos, int speed, int health, int direction, char core_character);
    void display(WINDOW*) const override;
    void moveUp(const LevelGrid&);
    void moveDown(const LevelGrid&);
    void moveLeft(const LevelGrid&);
    void moveRight(const LevelGrid&);
    pair<int> getPos() const;
    virtual void updateHealth(int);
    void updateWallsTouched(const LevelGrid&) override; //I think this is useless actually we'll see
    void checkWalls(const LevelGrid&, bool, bool, bool, bool);
protected:
    unsigned int health_;
    char core_character_;
};

//character 1
//mister regular is just the normal character
class MisterRegular : public PlayerCharacter {
public:
    MisterRegular(pair<int> pos, int speed, int health, int direction, char core_character = 'O');
};

//character 2
//muscle man is slower but he has more health 
class MuscleMan : public PlayerCharacter {
public:
    MuscleMan(pair<int> pos, int speed, int health, int direction, char core_character = '8');
    void updateHealth(int) override;
};

//character 3
//shield bro has a shield that he can turn on for a limited time
class ShieldBro : public PlayerCharacter {
public:
    ShieldBro(pair<int> pos, int speed, int health, int direction, int shield_count, char core_character = 'H', bool shield = false);
    void updateHealth(int) override;
    void setShield(bool); //checks internally if the player already used all his/her shields
    static int MAX_SHIELD_COUNT;
private:
    bool shield_;
    int shield_count_;
};

//character 4
//sneaky lady can be undetectable state that she can turn on for a limited time
class SneakyLady : public PlayerCharacter {
public:
    SneakyLady(pair<int> pos, int speed, int health, int sneak_count, char core_character = 'S', bool sneak_status = false);
    void updateHealth(int) override;
    void setSneakStatus(bool); //checks internally if the player has used all his sneak powerups
    static int MAX_STEALTH_COUNT;
private:
    bool sneak_status_;
    int sneak_count_;
};