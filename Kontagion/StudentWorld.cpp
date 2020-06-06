#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <cmath>
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    
}

int StudentWorld::init()
{
    Actor* player = new Socrates(this);
    add(player);
    int L= getLevel();
    int noOfDirt= max(180-20 * L, 20);
    for (int i=0; i<noOfDirt; i++)
    {
        int x=0, y=0;
        getRandomPosition(x, y);
        Actor* dirt= new Dirt(x, y, this);
        add(dirt);
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    for (int i=0; i<m_activeObjects.size(); i++)
        m_activeObjects[0]->doSomething();
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::getRandomPosition(int &x, int &y)
{
    int r = randInt(0, 120);
    Direction theta = randInt(0, 359);
    x= VIEW_WIDTH/2 + r*cos(theta);
    y= VIEW_HEIGHT/2 + r*sin(theta);
}

void StudentWorld::cleanUp()
{
    while(m_activeObjects.size()>0)
    {
        delete m_activeObjects[m_activeObjects.size()-1];
        m_activeObjects.pop_back();
    }
}

void StudentWorld::add(Actor* a)
{
    m_activeObjects.push_back(a);
}
