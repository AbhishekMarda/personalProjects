#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <cmath>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{}

int StudentWorld::init()
{
    m_bacteriaLeft=0;
    m_pitsLeft=0;
    Socrates* player = new Socrates(this);                                                              //create the player
    add(player);
    m_player = player;
    int L= getLevel();
    
    for (int i=0; i<L; i++)                                                                             //add pits
    {
        int x=0, y=0;
        Actor *pit= new Pit(x,y,this);
        do
        {
            getRandomPosition(x, y);
            pit->moveTo(x, y);
        }while(hasAnyOverlap(pit, (int) m_activeObjects.size()));                                       //keep searching for random positions until it is within 120 pixels of the center
        add(pit);
        m_pitsLeft++;
    }

    int noOfFood= min(5*L, 25);                                                                          //add food
    for (int i=0; i<noOfFood; i++)
    {
        int x=0, y=0;
        Actor *food = new Food(x, y, this);
        do
        {
            getRandomPosition(x, y);
            food->moveTo(x,y);
        } while(hasAnyOverlap(food, (int) m_activeObjects.size()));                                     //keep searching for random positions until it is within 120 pixels of the center
        add (food);
    }
    
    int noOfDirt= max(180-20 * L, 20);                                                                  //add dirt
    int currLen = (int) m_activeObjects.size();                                                         //dirts are allowed to overlap, so we need to check for overlap with only currLen elements
    for (int i=0; i<noOfDirt; i++)
    {
        int x=0, y=0;
        Actor* dirt= new Dirt(x, y, this);
        do
        {
            getRandomPosition(x, y);
            dirt->moveTo(x, y);
        } while (hasAnyOverlap(dirt, currLen));                                                         //keep searching for random positions until it is within 120 pixels of the center
        add(dirt);
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
            
    for (int i=0; i<m_activeObjects.size(); i++)                                                        //call the doSomething() function for every alive object
    {
        if (m_activeObjects[i]->isAlive())
            m_activeObjects[i]->doSomething();
        
        if (!m_activeObjects[0]->isAlive())                                                             //if socrates dies, game ends
        {
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
        
        if (levelWon())                                                                                 //if game is won, go to next level
        {
            playSound(SOUND_FINISHED_LEVEL);
            return GWSTATUS_FINISHED_LEVEL;
        }
    }
    
    for (int i=0; i<m_activeObjects.size();)                                                            //remove all dead objects
    {
        if (!m_activeObjects[i]->isAlive())
        {
            delete m_activeObjects[i];
            m_activeObjects[i]= m_activeObjects[m_activeObjects.size()-1];
            m_activeObjects.pop_back();
        }
        else
            i++;
    }
    
                                                                    //adding new objects
                                                                                                        //adding fungus
    int L= getLevel();
    int chanceFungus = min(510 - L * 10, 200);
    const double PI = 4 * atan(1);
    int fungusChance = randInt (0, chanceFungus-1);
    if (fungusChance==0)
    {
        int newAngle= randInt(0, 359);                                                                  //randomly get point on the perimeter
        
        int x= VIEW_RADIUS * cos(newAngle*1.0 / 360 * 2 * PI) + VIEW_WIDTH/2;
        int y= VIEW_RADIUS * sin(newAngle*1.0 / 360 * 2 * PI) + VIEW_HEIGHT/2;
        int lifetime=max(rand() % (300 - 10 * L), 50);
        Actor* fungus= new Fungus(x, y, this, lifetime);
        add(fungus);
    }
    
                                                                                                        //adding goodies
    int ChanceGoodie = min(510 - L * 10, 250);
    int goodieChange= randInt (0, ChanceGoodie-1);
    if (goodieChange==0)
    {
        Actor* newGoodie=nullptr;
        int goodieType= randInt(0,9);
        int newAngle = randInt(0, 359);
        int x= VIEW_RADIUS * cos(newAngle*1.0 / 360 * 2 * PI) + VIEW_WIDTH/2;
        int y= VIEW_RADIUS * sin(newAngle*1.0 / 360 * 2 * PI) + VIEW_HEIGHT/2;
        if (goodieType<6)                                                                               //60% chance of restore health goodie
        {
            int lifetime=max(rand() % (300 - 10 * L), 50);
            newGoodie= new RestoreHealth(x, y, this, lifetime);
        }
        else if (goodieType<9)                                                                          //30& chance of flame thrower goodie
        {
            int lifetime=max(rand() % (300 - 10 * L), 50);
            newGoodie= new FlameThrowerCharge(x, y, this, lifetime);
        }
        else if (goodieType<10)
        {
            int lifetime=max(rand() % (300 - 10 * L), 50);                                              //10% chance of extra life goodie
            newGoodie= new ExtraLife(x, y, this, lifetime);
        }
        add(newGoodie);
    }
    
    ostringstream output;
    output.fill('0');
    output << "Score: ";
    if (getScore()>=0)
        output <<setw(6)<<getScore();
    else
        output <<"-"<<setw(5)<< -1 * getScore();
    output <<" Level: "<<getLevel();
    output <<" Lives: "<<getLives();
    output <<" health: "<<m_player->getHealth();
    output <<" Sprays: "<<m_player->getSprays();
    output <<" Flames: "<<m_player->getFlameThrowerCharges();
    
    string display= output.str();
    setGameStatText(display);
    

    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    while(m_activeObjects.size()>0)                                                                     //avoid memory leak by deleting the pointers of all actors
    {
        delete m_activeObjects[m_activeObjects.size()-1];
        m_activeObjects.pop_back();
    }
}

void StudentWorld::add(Actor* a)
{
    m_activeObjects.push_back(a);
}

bool StudentWorld::isOverlap(Actor *a1, Actor *a2, int radius) const                                    //determine if there a maximum of specified radius between the two actors
{
    int x1= a1->getX(),
        y1= a1->getY(),
        x2= a2->getX(),
        y2= a2->getY();
    
    double eucDist= sqrt(pow(x1-x2,2)+pow(y1-y2, 2));
    
    return eucDist<=radius;
}

bool StudentWorld::hasAnyOverlap(Actor *a,int noOfStartingElements, int radius) const                   //determine if there is an overlap for specified number of starting elements with the provided actor in the specified radius
{
    
    for (int i=0; i<noOfStartingElements; i++)
    {
        if (isOverlap(a, m_activeObjects[i],radius))
            return true;
    }
    return false;
    
}

bool StudentWorld::bacteriaDamageableOverlap(Actor *a)                                                  //damaage an object that is damageable by a bacteria and is overlapping with it
{
    for (int i=0; i<m_activeObjects.size(); i++)
    {
        if (isOverlap(a, m_activeObjects[i]) && m_activeObjects[i]->isDamageableByBacteria())
        {
            m_activeObjects[i]->damage(0);
            return true;
        }
    }
    return false;
}


bool StudentWorld::hasAnyOverlap(Actor *a) const                                                        //see whether an element has any overlap across the entire vector of actors
{
    return hasAnyOverlap(a, (int)m_activeObjects.size());
}

void StudentWorld::getRandomPosition(int &x, int &y) const
{
    int eucDist=0;
    do{
        x= randInt(0, VIEW_WIDTH);
        y= randInt(0, VIEW_HEIGHT);
        eucDist=sqrt(pow(x- VIEW_WIDTH/2,2)+pow(y - VIEW_HEIGHT/2, 2));
    }
    while (eucDist>120);                                                                                //get a random position within 120 pixels (this function is only called for the initialization of food, dirt, and pit type objects in StudentWorld::inti()
}


bool StudentWorld::projectileDamageableOverlap(Actor *a, int amount)                                    //damage an object that is damageable by a bacteria and is overlapping with it
{
    for (int i=0; i<m_activeObjects.size(); i++)
    {
        if (isOverlap(a, m_activeObjects[i]) && m_activeObjects[i]->isDamageableByProjectile())
        {
            m_activeObjects[i]->damage(amount);
            return true;
        }
    }
    return false;
}

void StudentWorld::setPlayerHealth(int hp)                                                              //see the player's health to the specified amount
{
    m_player->setHealth(hp);
}

void StudentWorld::pickUpFlamethrowerCharge()                                                           //make socrates add 5 flame charges to its inventory
{
    m_player->flamethrowerRecharge();
}

int StudentWorld::getPlayerHealth() const
{
    return m_player->getHealth();
}


bool StudentWorld::bacteriaIsBlocked(Actor *a, int radius) const                                        //check if the bacteria is blocked
{
    for (int i=0; i<m_activeObjects.size(); i++)
    {
        if (isOverlap(a, m_activeObjects[i], radius) && m_activeObjects[i]->isObstacle())
            return true;
    }
    
    return false;
}

bool StudentWorld::getNearestFood (Actor *a, int& x, int &y, int radius) const                          //find nearet food object in the specified radius
{
    if (radius>VIEW_RADIUS)
        radius=VIEW_RADIUS;
    for (int j=SPRITE_WIDTH/2; j<radius; j++)
        for (int i=1; i<m_activeObjects.size(); i++)                                                    //check from second postion onwards (all actors excluding socrates)
            if (isOverlap(a, m_activeObjects[i], j) && m_activeObjects[i]->isDamageableByBacteria())
            {
                x=m_activeObjects[i]->getX();
                y=m_activeObjects[i]->getY();
                return true;
            }
    return false;
}

void StudentWorld::socratesPos(int &x, int &y) const                                                    //pass socrates' position into the variables passed by reference
{
    x= m_player->getX();
    y= m_player->getY();
}

void StudentWorld::addNoOfBacteria(int n)                                                               //increase the count of bacteria
{
    if (m_bacteriaLeft + n >= 0)
        m_bacteriaLeft+=n;
}

void StudentWorld::decrementPits()
{
    m_pitsLeft--;
}

bool StudentWorld::levelWon() const                                                                     //evaluate if the level was won
{
    return (m_pitsLeft==0 && m_bacteriaLeft==0);
}

void StudentWorld::damagePlayer(int damage)                                                             //damage player by specified amount
{
    m_player->damage(damage);
}
