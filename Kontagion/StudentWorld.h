#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    ~StudentWorld() {cleanUp();}
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    void getRandomPosition(int &x, int &y);
    void add(Actor* a);                     //self defined
private:
    std::vector<Actor*> m_activeObjects;
};

#endif // STUDENTWORLD_H_
