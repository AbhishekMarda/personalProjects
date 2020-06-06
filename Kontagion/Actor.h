#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
const int MAX_SPRAY_CHARGES=20;
const int INIT_FIRE_CHARGES=5;

class Socrates;
class Spray;
class StudentWorld;
class Dirt;
class Actor: public GraphObject
{
public:
    Actor(int imageID, int startCol, int startRow, Direction facing, int depth, StudentWorld* world) : GraphObject(imageID, startCol, startRow, facing, depth)
    {
        m_isAlive = true;                                               //player starts out alive
        m_world = world;
    }
    virtual ~Actor() {}
    
    //auxiliary actions
    virtual void doSomething();
    
    //getters
    StudentWorld* getStudentWorldPtr();
    bool isAlive();

    //setters
    void setDead();
    
private:
    bool m_isAlive;
    StudentWorld* m_world;
};
//////////////////////////////////////////////////////////////////////////////

class Socrates : public Actor
{
public:
    Socrates(StudentWorld* world) : Actor (IID_PLAYER, 0, VIEW_HEIGHT/2, 0, 0, world)
    {
        m_health=100;                                                           //starts out with full health
        m_sprayCharges=MAX_SPRAY_CHARGES;                                       //initializing spray charges
        m_flameThrowerCharges=INIT_FIRE_CHARGES;                                //initializing flamethrower charges
    }
    virtual void doSomething();
    
private:
    int m_health;
    int m_sprayCharges;
    int m_flameThrowerCharges;

};
//////////////////////////////////////////////////////////////////////////////

class Spray : public Actor
{
public:
    Spray(int col, int row, Direction dir, StudentWorld* world) : Actor(IID_SPRAY, col, row, dir, 1, world)
    {
        moveAngle(dir, 2*SPRITE_RADIUS);
    }
    virtual void doSomething();
};
//////////////////////////////////////////////////////////////////////////////
class Flame : public Actor
{
public:
    Flame (int col, int row, Direction dir, StudentWorld* world) : Actor(IID_FLAME, col, row, dir, 1, world)
    {
        moveAngle(dir, 2*SPRITE_RADIUS);
    }
    
    virtual void doSomething();
};

//////////////////////////////////////////////////////////////////////////////

class Dirt: public Actor
{
public:
    Dirt(int col, int row, StudentWorld* world) : Actor (IID_DIRT, col, row, 0, 1, world)
    {
    }
};

#endif // ACTOR_H_
