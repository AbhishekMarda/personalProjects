#ifndef ACTOR_H_
#define ACTOR_H_
#include "GraphObject.h"



// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Socrates;
class Projectile;
class Spray;
class Flame;
class StudentWorld;
class Dirt;
class Food;
class Goodies;
class RestoreHealth;
class FlameThrowerCharge;
class ExtraLife;
class Fungus;
class Bacteria;
class RegularSalmonella;
class AggressiveSalmonella;
class EColi;
class Pit;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ACTOR BASE CLASS DECLARATION //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Actor: public GraphObject
{
public:
    Actor(int imageID, int startCol, int startRow, Direction facing, int depth, StudentWorld* world);
    virtual ~Actor(){}                                                                 //virtual destructor to avoid memory leaks
    
    //accessors
    StudentWorld* getStudentWorldPtr() const;
    bool isAlive() const;
    virtual bool isDamageableByProjectile() const;
    virtual bool isDamageableByBacteria() const;
    virtual bool isObstacle() const;
 
    //mutators
    void setDead();
    virtual void damage(int amount);
    virtual void doSomething()=0;
    
private:
    bool m_isAlive;
    StudentWorld* m_world;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// SOCRATES DECLARATION ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Socrates : public Actor
{
public:
    Socrates(StudentWorld* world);
    
    //accessors
    virtual bool isDamageableByBacteria() const;
    virtual bool isDamageableByProjectile() const;
    int getHealth() const;
    int getFlameThrowerCharges() const;
    int getSprays() const;
    
    //mutators
    virtual void doSomething();
    virtual void damage(int amount);
    void setHealth(int hp);
    void flamethrowerRecharge();
    
    
private:
    int m_health;
    int m_sprayCharges;
    int m_flameThrowerCharges;
    const int MAX_SPRAY_CHARGES=20;
    const int INIT_FIRE_CHARGES=5;

};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// PROJECTILE DECLARATION /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Projectile : public Actor
{
public:
    Projectile(int imageID, int col, int row, Direction dir, StudentWorld* world, int maxDistance, int damage);
    
    //accessor
    virtual bool isDamageableByProjectile() const;
    
    //mutators
    virtual void doSomething();
    void moveForward();
    
private:
    int m_maxDistance;
    int m_distanceTravelled;
    int m_damage;
};


class Spray : public Projectile
{
public:
    Spray(int col, int row, Direction dir, StudentWorld* world);
};

class Flame : public Projectile
{
public:
    Flame (int col, int row, Direction dir, StudentWorld* world);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// DIRT DECLARATION ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Dirt: public Actor
{
public:
    Dirt(int col, int row, StudentWorld* world);
    
    //accessor
    virtual bool isObstacle() const;
    
    //mutator
    virtual void doSomething();
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// FOOD DECLARATION /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Food: public Actor
{
public:
    Food(int col, int row, StudentWorld* world);
    
    //accessors
    virtual bool isDamageableByProjectile() const;
    virtual bool isDamageableByBacteria() const;
    
    //mutator
    virtual void doSomething();
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// GOODIES DECLARATION ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Goodies: public Actor
{
public:
    Goodies (int image, int col, int row, StudentWorld* world, int lifetime, int points);
    
    //mutators
    virtual void doSomething();
    virtual void doAbility()=0;
private:
    int m_lifetime;
    int m_points;
};

class RestoreHealth: public Goodies
{
public:
    RestoreHealth (int col, int row, StudentWorld* world, int lifetime);
    virtual void doAbility();
};

class FlameThrowerCharge: public Goodies
{
public:
    FlameThrowerCharge (int col, int row, StudentWorld* world, int lifetime);
    virtual void doAbility();
};

class ExtraLife: public Goodies
{
public:
    ExtraLife(int col, int row, StudentWorld* world, int lifetime);
    virtual void doAbility();
    
};

class Fungus: public Goodies
{
public:
    Fungus(int col, int row, StudentWorld* world, int lifetime);
    virtual void doAbility();
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// BACTERIA DECLARATION ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Bacteria : public Actor
{
public:
    Bacteria(int image, int col, int row, StudentWorld* world, int health, int damage, int killpoints);
        
    //accessors
    virtual void hurtSound() const;
    virtual void deadSound() const;
    int getMovementPlan () const;
    
    //mutators
    virtual void doSomething();
    virtual void damage(int amount);
    void divide();
    virtual void duplicate(int col, int row)=0;
    bool tryToMoveInDir (int px, int overlapRadius=SPRITE_WIDTH/2);
    bool targetNearestFood (int radius);
    bool targetSocrates (int radius);
    virtual bool becomeHeatseeker();
    virtual void changeMovement();
    void setRandomDirection();
    void setMovementPlan(int newDir);
    
    
    
private:
    int m_health;
    int m_damage;
    int m_foodEaten;
    int m_movementPlanDistance;
    int m_killpoints;
    const double PI= 3.14159265;
};

//                                                       REGULAR SALMONELLA

class RegularSalmonella : public Bacteria
{
public:
    RegularSalmonella(int col, int row, StudentWorld* world);
    virtual void duplicate(int col, int row);
};

//                                                      AGGRESSIVE SALMONELLA
class AggressiveSalmonella : public Bacteria
{
public:
    AggressiveSalmonella(int col, int row, StudentWorld* world);
    virtual void duplicate(int col, int row);
    virtual bool becomeHeatseeker();

};
//                                                              ECOLI
class EColi : public Bacteria
{
public:
    EColi(int col, int row, StudentWorld* world);
    virtual void duplicate(int col, int row);
    virtual void changeMovement();
    virtual void hurtSound();
    virtual void deadSound();
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// PIT DECLARATION ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class Pit: public Actor
{
public:
    Pit(int col, int row, StudentWorld* world);
    
    //accessors
    virtual bool isDamageableByProjectile() const;
    bool empty() const;
    
    //mutators
    void releaseRegularSalmonella();
    void releaseAggressiveSalmonella();
    void releaseEColi();
    void releaseRandomBacteria();
    virtual void doSomething();
    
private:
    int m_regularSalmonella;
    int m_aggressiveSalmonella;
    int m_ecoli;
};
#endif // ACTOR_H_
