#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// ACTOR IMPLEMENTATION //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Actor::Actor(int imageID, int startCol, int startRow, Direction facing, int depth, StudentWorld* world): GraphObject(imageID, startCol, startRow, facing, depth)

{
    m_isAlive = true;
    m_world = world;
}

StudentWorld* Actor::getStudentWorldPtr() const
{
     return m_world;
}

bool Actor::isAlive() const
{
    return m_isAlive;
}

bool Actor::isDamageableByProjectile() const                                                        //determines if a an object can be damaged by a projectile
{
    return true;
}

void Actor::setDead()                                                                               //sets the objects status as dead
{
    m_isAlive=false;
}

bool Actor::isObstacle() const                                                                      //for objects for which this is true, the bacteria can not pass it
{
    return false;
}

bool Actor::isDamageableByBacteria() const                                                          //determines whether an object can be damaged by a projectile on overlap
{
    return false;
}

void Actor::damage(int amount)                                                                      //damages in different specialized ways, but by default sets an object dead
{
    setDead();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// SOCRATES IMPLEMENTATION ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

Socrates::Socrates(StudentWorld* world) : Actor(IID_PLAYER, 0, VIEW_HEIGHT/2, 0, 0, world)
   {
       m_health=100;
       m_sprayCharges=MAX_SPRAY_CHARGES;
       m_flameThrowerCharges=INIT_FIRE_CHARGES;
   }

void Socrates::doSomething()
{
    if (!isAlive())
        return;
    int key=0;
    if (getStudentWorldPtr()->getKey(key))
    {
        switch (key)
        {
            case KEY_PRESS_SPACE:                                                                   //disinfectant spray will be fired
            {
                if (m_sprayCharges>0)                                                               //don't fire if no more spray charges are left
                {
                    double x, y;
                    getPositionInThisDirection(getDirection(), SPRITE_WIDTH, x, y);                 //spray object has to be created SPRITE_WIDTH in front of the player
                    Actor* ptr= new Spray(x, y, getDirection(), getStudentWorldPtr());
                    getStudentWorldPtr()->add(ptr);
                    m_sprayCharges--;
                getStudentWorldPtr()->playSound(SOUND_PLAYER_SPRAY);
                    
                } 
                break;
            case KEY_PRESS_ENTER:                                                                   //flamethrower will be usesd
                {
                    if (m_flameThrowerCharges>0)                                                    //don't fire if no more charges are left
                    {
                        for (int i=0, deg=0; i<16; i++, deg+=22)                                    //16 flames have to be fired at 22 degrees
                        {
                            double x, y;
                            getPositionInThisDirection(getDirection()+deg, SPRITE_WIDTH, x, y);     //in their respective directions, the flame objects should be created SPRITE_WIDTH distance from the player
                            Actor* ptr= new Flame(x, y, getDirection()+deg, getStudentWorldPtr());
                            getStudentWorldPtr()->add(ptr);
                        }
                        m_flameThrowerCharges--;
                        getStudentWorldPtr()->playSound(SOUND_PLAYER_FIRE);
                    }
                }
                break;
            case KEY_PRESS_LEFT:                                                                    //move anticlockwise
                {                                                                                   //this is done by the following process:
                    moveTo(VIEW_WIDTH/2, VIEW_HEIGHT/2);                                            //move to the center
                    int newDir= getDirection()+5;                                                   //change the direciton by the required amount
                    if (newDir>359)                                                                 //wrap around a whole angle
                        newDir-=360;
                    setDirection(newDir);                                                           //set the direction
                    moveAngle(newDir, -VIEW_RADIUS);                                                //move back by the radius
                }
                break;
            case KEY_PRESS_RIGHT:                                                                   //move clockwise
                {                                                                                   //same process as pressing the left key press, direction change in opposite direction
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
    {                                                                                               //user did not play anything
        if (m_sprayCharges<MAX_SPRAY_CHARGES)                                                       //so increment spray charges till the limit
        m_sprayCharges++;
    }
}

void Socrates::damage(int amount)
{
    m_health-=amount;                                                                               //decrement health by the required amount
    if (m_health<=0)                                                                                //kill Socrates if health is less than 0
    {
        setDead();
        getStudentWorldPtr()->playSound(SOUND_PLAYER_DIE);
    }
    else
        getStudentWorldPtr()->playSound(SOUND_PLAYER_HURT);
}

bool Socrates::isDamageableByBacteria() const
{
    return true;
}

void Socrates::setHealth(int hp)
{
    if (hp>100 || hp<0)                                                                             //health can only be between 0 and 100
        return;
    m_health=hp;
    if (hp==0)
        setDead();
}

int Socrates::getHealth() const
{
    return m_health;
}

void Socrates::flamethrowerRecharge()
{
    m_flameThrowerCharges+=5;
}

int Socrates::getSprays() const
{
    return m_sprayCharges;
}

int Socrates::getFlameThrowerCharges() const
{
    return m_flameThrowerCharges;
}

bool Socrates::isDamageableByProjectile() const
{
    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// PROJECTILE IMPLEMENTATION ///////////////////////////////////////////////////////////////////////////////////////////////////////

Projectile::Projectile(int imageID, int col, int row, Direction dir, StudentWorld* world, int maxDistance, int damage) : Actor (imageID, col, row, dir, 1, world)
   {
       m_maxDistance = maxDistance;
       m_damage=damage;
       m_distanceTravelled=0;
   }

bool Projectile::isDamageableByProjectile() const
{
    return false;
}

void Projectile::doSomething()
{
    if (!isAlive())
        return;
    
    if (getStudentWorldPtr()->projectileDamageableOverlap(this, m_damage))                          //damage an object that overlaps with the projectile and can be damaged by it
    {
        setDead();                                                                                  //projectile dies when it damages something
        return;
    }
    moveForward();                                                                                  //if it didn't damage anything, move it forward
}

void Projectile::moveForward()
{
    moveAngle(getDirection(), SPRITE_WIDTH);                                                        //move required amount in the current direction
    m_distanceTravelled+=SPRITE_WIDTH;
    
    if (m_maxDistance<=m_distanceTravelled)                                                         //remove the projectile once it has moved its maximum distance
        setDead();
}
Spray::Spray(int col, int row, Direction dir, StudentWorld* world) : Projectile(IID_SPRAY, col, row, dir, world, 112, 2)
{
   // moveAngle(dir, SPRITE_WIDTH);
}

Flame::Flame (int col, int row, Direction dir, StudentWorld* world) : Projectile(IID_FLAME, col, row, dir, world, 32, 5)
{
  //  moveAngle(dir, SPRITE_WIDTH);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////// DIRT DECLARATION ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


Dirt::Dirt(int col, int row, StudentWorld* world) : Actor (IID_DIRT, col, row, 0, 1, world)
   {
   }

void Dirt::doSomething()
{
    return;
}

bool Dirt::isObstacle() const
{
    return true;                                                                                    //blocks bacteria from moving through it
}
/////////////////////////////////////////////////////////////////////////////////////////////////////// FOOD DECLARATION ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Food::Food(int col, int row, StudentWorld* world) : Actor (IID_FOOD, col, row, 90, 1, world)
{
}

void Food::doSomething()
{
    return;
}

bool Food::isDamageableByBacteria() const
{
    return true;
}

bool Food::isDamageableByProjectile() const
{
    return false;
}


////////////////////////////////////////////////////////////////////////////////////////////////// GOODIES IMPLEMENTATION /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Goodies::Goodies (int image, int col, int row, StudentWorld* world, int lifetime, int points) : Actor (image, col, row, 0, 1, world)
{
    m_lifetime=lifetime;
    m_points=points;
}

void Goodies::doSomething()
{
    if (!isAlive())
        return;
    
    
    if (getStudentWorldPtr()->hasAnyOverlap(this, 1))                                           //check for overlap with socrates which is at the first (0th) index in the vector of actors in StudentWorld
    {
        getStudentWorldPtr()->increaseScore(m_points);                                          //increase by required number of points
        setDead();                                                                              //can only work once, so once used, it should be removed
        if (m_points>0)
            getStudentWorldPtr()->playSound(SOUND_GOT_GOODIE);
        doAbility();                                                                            //this is specific to every goodie
        return;
    }
    
    m_lifetime--;
    if (m_lifetime<=0)
        setDead();
    
}


RestoreHealth::RestoreHealth (int col, int row, StudentWorld* world, int lifetime) : Goodies (IID_RESTORE_HEALTH_GOODIE, col, row, world, lifetime, 250) {}

void RestoreHealth::doAbility()
{
    getStudentWorldPtr()->setPlayerHealth(100);                                                 //refresh the players health
}

FlameThrowerCharge::FlameThrowerCharge (int col, int row, StudentWorld* world, int lifetime) : Goodies (IID_FLAME_THROWER_GOODIE, col, row, world, lifetime, 300) {}

void FlameThrowerCharge::doAbility()
{
    getStudentWorldPtr()->pickUpFlamethrowerCharge();                                           //increment required number of flame thrower charges
}

ExtraLife::ExtraLife(int col, int row, StudentWorld* world, int lifetime) :Goodies(IID_EXTRA_LIFE_GOODIE, col, row, world, lifetime, 500){}

void ExtraLife::doAbility()
{
    getStudentWorldPtr()->incLives();                                                           //increase the number of lives
}

Fungus::Fungus(int col, int row, StudentWorld* world, int lifetime) : Goodies(IID_FUNGUS, col, row, world, lifetime, -50){}

void Fungus::doAbility()
{
    getStudentWorldPtr()->damagePlayer(20);                                                     //cause a damage of 20 hitpoints
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////// BACTERIA IMPLEMENTATION ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Bacteria::Bacteria(int image, int col, int row, StudentWorld* world, int health, int damage, int killpoints) : Actor(image, col, row, 90, 0, world)
 {
     m_health=health;
     m_damage=damage;
     m_foodEaten=0;
     m_movementPlanDistance=0;
     m_killpoints= killpoints;
     getStudentWorldPtr()->addNoOfBacteria(1);                                                  //tell StudentWorld that there is another bacteria created
     getStudentWorldPtr()->playSound(SOUND_BACTERIUM_BORN);
 }

void Bacteria::damage(int amount)
{
    if (m_health<=0)
        return;
    m_health-=amount;
    if (m_health<=0)                                                                            //bacteria dies when its health is less than or equal to 0
    {
        int chanceOfBecomingFood = randInt(0, 1);                                               //there is a 50% chance that the bacteria will become food
        if (chanceOfBecomingFood)
        {
            Actor *food= new Food(getX(), getY(), getStudentWorldPtr());                        //food object is created at the same coordinates as the bacteria
            getStudentWorldPtr()->add(food);
        }
        deadSound();
        getStudentWorldPtr()->increaseScore(m_killpoints);
        getStudentWorldPtr()->addNoOfBacteria(-1);
        setDead();
    }
    else
    {
        hurtSound();
    }
}

void Bacteria::doSomething()
{
    if (!isAlive())
        return;
    
    bool playerSensed=becomeHeatseeker();                                                       //in case of aggressive salmonella only, this line makes the bacteria move in direction of Socrates if sensed in 72px
    
    if (getStudentWorldPtr()->hasAnyOverlap(this, 1))                                           //damage player if there is overlap with the player
        getStudentWorldPtr()->damagePlayer(m_damage);
    
    else if (m_foodEaten==3)                                                                    //divide the bacteria once it eats 3 food objects
    {
        divide();
        m_foodEaten=0;                                                                          //reset the food eaten once bacteria created another one
    }
    else
    {
        if(getStudentWorldPtr()->bacteriaDamageableOverlap(this))                               //check if there is overlap with a food object and increase the number of food items eaten
        {
            m_foodEaten++;
        }
    }
    if (playerSensed)                                                                           //this ensrues that if an aggressive salmonella is moving toward a player, it isn't moving toward a food item as well
        return;
    else
        changeMovement();                                                                       //specifc to every bacteria type, this changes the movement of the bacteria according to the required behavior
}

bool Bacteria::becomeHeatseeker()
{
    return false;                                                                               //changes only for aggressive salmonella
}

void Bacteria::setMovementPlan(int newDir)
{
    m_movementPlanDistance=newDir;
}

int Bacteria::getMovementPlan() const
{
    return m_movementPlanDistance;
}

bool Bacteria::tryToMoveInDir(int px, int overlapRadius)
{
    moveAngle(getDirection(), px);                                                              //move temporarily in the the given radius
    double currRad = sqrt(pow(getX()-VIEW_WIDTH/2, 2) + pow(getY()-VIEW_HEIGHT/2, 2));          //euclidean distance between center and bacteria
    if (!getStudentWorldPtr()->bacteriaIsBlocked(this, overlapRadius) && currRad<VIEW_RADIUS)   //make the movement permanent if its not blocked and it is within the bounds of the petri dish
        return true;
    
    moveAngle(getDirection(), -px);                                                             //if the bacteria is blocked or moves out of the petri dish, move back
    return false;
}

bool Bacteria::targetNearestFood(int radius)                                                    //this function changes direction to the nearest food item
{
    int targetX=0, targetY=0;
    bool wasFound = getStudentWorldPtr()->getNearestFood(this, targetX, targetY, radius);       //get coordinates of the nearest food item, if there is one in the specified radius
    if (wasFound)
    {
        int xdist = targetX-getX();
        int ydist= targetY-getY();
        Direction newDir;
        if (xdist==0 && ydist>0)
            newDir=90;
        else if (xdist==0 && ydist<0)
            newDir=270;
        else
            newDir = atan2(ydist, xdist) * 180 / PI;
        setDirection(newDir);                                                                   //set the direction based on the coordinates of the nearest food item, provided there is one in the nearest radius
    }
    
    return wasFound;
}

bool Bacteria::targetSocrates(int radius)
{
    int targetX=0, targetY=0;
    if (!getStudentWorldPtr()->hasAnyOverlap(this, 1, radius))                                  //check if Socrates is within the specified radius
        return false;
    getStudentWorldPtr()->socratesPos(targetX, targetY);                                        //get position of socrates
    
    int xdist= targetX- getX();
    int ydist= targetY-getY();
    Direction newDir;
    if (xdist==0 && ydist>0)
        newDir=90;
    else if (xdist==0 && ydist<0)
        newDir=270;
    else
        newDir = atan2(ydist, xdist) * 180 / PI;
    setDirection(newDir);                                                                       //set the new direction based on the coordinates of Socrates
    
    return true;
}

void Bacteria::setRandomDirection()
{
    Direction newDir= randInt(0, 359);
    setDirection(newDir);
    m_movementPlanDistance=10;
}

void Bacteria::divide()
{
    // get the coordinates of the new bacteria to be created based on the current coordinates of the bacteria
    int newX=getX() + SPRITE_WIDTH/2;
    int newY=getY() + SPRITE_WIDTH/2;
    
    if (getX() > VIEW_WIDTH/2)
        newX-= SPRITE_WIDTH;
    else if (getX()==VIEW_WIDTH/2)
        newX=getX();
    
    if (getY() > VIEW_HEIGHT/2)
        newY-= SPRITE_WIDTH;
    else if (getY()==VIEW_HEIGHT/2)
        newY=getY();
    
    
    duplicate(newX, newY);                                                                      //create a new bacteria item in the described coordinates
    
}

void Bacteria::changeMovement()                                                                 //this is the default definition of the function which is used for both salmonella
{
    if (getMovementPlan()>0)                                                                    //there is already an existing movement plan
    {
        setMovementPlan(getMovementPlan()-1);
        
        if (!tryToMoveInDir(3, SPRITE_WIDTH/2))                                                 //move in the specified direction given there is no overlap in SPRITE_WIDTH/2 pixels
        {
            setRandomDirection();                                                               //move in a random direction if the bacteria can no longer move in its current direction
        }
    }
    else
    {
        if (targetNearestFood(128))                                                             //if there is no movement plan, set direction to nearest food item in 128 pixels
        {
            if (!tryToMoveInDir(3, SPRITE_WIDTH/2))                                                             //attempt to move in the new direction
                setRandomDirection();                                                           //and if not possible, then move in a random direction
        }
        else
            setRandomDirection();                                                               //no movement plan or food item in sight, then move in a random direction
    }
}

void Bacteria::hurtSound() const
{
    getStudentWorldPtr()->playSound(SOUND_SALMONELLA_HURT);
}

void Bacteria::deadSound() const
{
    getStudentWorldPtr()->playSound(SOUND_SALMONELLA_DIE);
}

//                                                           REGULAR SALMONELLA

RegularSalmonella::RegularSalmonella(int col, int row, StudentWorld* world) : Bacteria (IID_SALMONELLA, col, row, world, 4, 1, 100){}

void RegularSalmonella::duplicate(int col, int row)
{
    Actor* rSal = new RegularSalmonella(col, row, getStudentWorldPtr());
    getStudentWorldPtr()->add(rSal);
}

//                                                           AGGRESSIVE SALMONELLA

AggressiveSalmonella::AggressiveSalmonella(int col, int row, StudentWorld* world) : Bacteria(IID_SALMONELLA, col ,row, world, 10, 2, 100){}

void AggressiveSalmonella::duplicate(int col, int row)
{
    Actor* rSal = new AggressiveSalmonella(col, row, getStudentWorldPtr());
    
    getStudentWorldPtr()->add(rSal);
}

bool AggressiveSalmonella::becomeHeatseeker()                                                   //forget food and try to move toward socrates if found in a radius of 72 pixels
{
    bool playerSensed =targetSocrates(72);
    if (playerSensed)
        tryToMoveInDir(3, SPRITE_WIDTH/2);
    return playerSensed;
}

//                                                                 ECOLI

EColi::EColi(int col, int row, StudentWorld* world) : Bacteria (IID_ECOLI, col, row, world, 5, 4, 100){}

void EColi::duplicate(int col, int row)
{
    Actor* rSal = new EColi(col, row, getStudentWorldPtr());
    getStudentWorldPtr()->add(rSal);
}

void EColi::changeMovement()
{
    int i=0;
    if (targetSocrates(256))                                                                    //set direction to socrates if found in 256 pixels
    {
        while (!tryToMoveInDir(2, SPRITE_WIDTH/2) && i<10)                                                      //try to move toward socrates, and if not possible, change direction by 10 degrees until possible to move for a maximum of 10 tries
        {
            if (getDirection()+10>360)
                setDirection(getDirection()+10-360);
            else
                setDirection(getDirection()+10);
            i++;
        }
    }
}

void EColi::hurtSound()
{
    getStudentWorldPtr()->playSound(SOUND_ECOLI_HURT);
}

void EColi::deadSound()
{
    getStudentWorldPtr()->playSound(SOUND_ECOLI_DIE);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// PIT IMPLEMENTATION ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Pit::Pit(int col, int row, StudentWorld* world) : Actor (IID_PIT, col, row, 0, 1, world)
 {
     m_regularSalmonella=5;
     m_aggressiveSalmonella=3;
     m_ecoli=2;
 }

void Pit::releaseRegularSalmonella()                                                            //instantiate a regular salmonella on the petri dish
{
    if (m_regularSalmonella<=0)
        return;
    Actor* bacteria= new RegularSalmonella(getX(), getY(), getStudentWorldPtr());
    getStudentWorldPtr()->add(bacteria);
    m_regularSalmonella--;
}

void Pit::releaseAggressiveSalmonella()                                                         //instantiate an aggressive salmonella on the petri dish
{
    if (m_aggressiveSalmonella<=0)
        return;
    Actor* bacteria= new AggressiveSalmonella(getX(), getY(), getStudentWorldPtr());
    getStudentWorldPtr()->add(bacteria);
    m_aggressiveSalmonella--;
}
    
void Pit::releaseEColi()                                                                        //instantiate an ecoli on the petri dish
{
    if (m_ecoli<=0)
        return;
    Actor* bacteria= new EColi(getX(), getY(), getStudentWorldPtr());
    getStudentWorldPtr()->add(bacteria);
    m_ecoli--;
}

bool Pit::empty() const                                                                         //describe if the pit has released its inventory of bacteria entirely
{
    return (m_ecoli + m_aggressiveSalmonella + m_regularSalmonella ==0);
}

void Pit::doSomething()
{
    if (empty())                                                                                //remove itself from the petri dish once all the bacteria are released
    {
        setDead();
        getStudentWorldPtr()->decrementPits();
        return;
    }
    
    int chance = randInt(0, 49);                                                                //1 in 50 chance of relesase a random bacteria
    if (!chance)
        releaseRandomBacteria();
}

void Pit::releaseRandomBacteria()
{
    if (empty())
        return;
    bool release[3] = {false, false, false};
    
    if (m_regularSalmonella>0)
        release[0]=true;
    if (m_aggressiveSalmonella>0)
        release[1]=true;
    if (m_ecoli>0)
        release[2]=true;
    
    int index;
    
                                                                                                //only relesase a bacteria type which still exists in the inventory of the pit
    do
    {
        index = randInt(0, 2);                                                                  //randomly choose which out of the three type of existing bacteria to release
    }while (!release[index]);
    
    switch(index)
    {
        case 0: releaseRegularSalmonella(); return;
        case 1: releaseAggressiveSalmonella(); return;
        case 2: releaseEColi(); return;
    }
}

bool Pit::isDamageableByProjectile() const
{
    return false;
}
