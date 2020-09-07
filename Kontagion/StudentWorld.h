#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class Socrates;
class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    ~StudentWorld(){cleanUp();}
    
    //self defined functions:
    
    //accessors
    void socratesPos(int &x, int &y) const;
    bool getNearestFood (Actor *a, int& x, int &y, int radius) const;
    bool bacteriaIsBlocked (Actor *a, int radius) const;
    int getPlayerHealth() const;
    bool levelWon() const;
    void getRandomPosition(int &x, int &y) const;
    bool isOverlap (Actor *a1, Actor *a2, int radius =SPRITE_WIDTH) const;
    bool hasAnyOverlap (Actor *a, int noOfStartingElements, int radius = SPRITE_WIDTH) const;
    bool hasAnyOverlap (Actor *a) const;
    
    
    //mutators
    void add(Actor* a);
    bool projectileDamageableOverlap (Actor *a, int amount=0);
    void addNoOfBacteria(int n);
    void decrementPits();
    bool bacteriaDamageableOverlap (Actor *a);
    void setPlayerHealth(int hp);
    void pickUpFlamethrowerCharge();
    void damagePlayer(int damage);
    
    
    
private:
    std::vector<Actor*> m_activeObjects;
    Socrates* m_player;
    int m_bacteriaLeft;
    int m_pitsLeft;
    
};

#endif // STUDENTWORLD_H_
