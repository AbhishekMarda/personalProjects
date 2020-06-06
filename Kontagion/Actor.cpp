#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp



StudentWorld* Actor::getStudentWorldPtr()
{
     return m_world;
}

bool Actor::isAlive()
{
    return m_isAlive;
}

void Actor::doSomething()
{
    return;
}

//////////////////////////////////////////////////////////////////////////////////////////

void Socrates::doSomething()
{
    if (!isAlive())
        return;
    int key=0;
    if (getStudentWorldPtr()->getKey(key))
    {
        switch (key)
        {
            case KEY_PRESS_SPACE:
            {
                if (m_sprayCharges>0)
                {
                    Actor* ptr= new Spray(getX(), getY(), getDirection(), getStudentWorldPtr());
                    getStudentWorldPtr()->add(ptr);
                    m_sprayCharges--;
                getStudentWorldPtr()->playSound(SOUND_PLAYER_SPRAY);
                    
                }
                break;
            case KEY_PRESS_ENTER:
                {
                    if (m_flameThrowerCharges>0)
                    {
                        for (int i=0, deg=0; i<16; i++, deg+=22)
                        {
                            Actor* ptr= new Flame(getX(), getY(), getDirection()+deg, getStudentWorldPtr());
                            getStudentWorldPtr()->add(ptr);
                        }
                        m_flameThrowerCharges--;
                        getStudentWorldPtr()->playSound(SOUND_PLAYER_FIRE);
                    }
                }
                break;
            case KEY_PRESS_LEFT:
                {
                    moveTo(VIEW_WIDTH/2, VIEW_HEIGHT/2);
                    int newDir= getDirection()+5;
                    if (newDir>359)
                        newDir-=360;
                    setDirection(newDir);
                    moveAngle(newDir, -VIEW_RADIUS);
                }
                break;
            case KEY_PRESS_RIGHT:
                {
                    moveTo(VIEW_WIDTH/2, VIEW_HEIGHT/2);
                    int newDir= getDirection()-5;
                    if (newDir<0)
                        newDir+=360;
                    setDirection(newDir);
                    moveAngle(getDirection(), -VIEW_RADIUS);
                }
                break;
            default: break;
            }
        }
    }
    else
    {
        if (m_sprayCharges<MAX_SPRAY_CHARGES)
        m_sprayCharges++;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
void Spray::doSomething()
{
    return;
}

//////////////////////////////////////////////////////////////////////////////////////////

void Flame::doSomething()
{
    return;
}
