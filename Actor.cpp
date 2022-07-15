#include "Actor.h"
#include "StudentWorld.h"
#include "GraphObject.h"
#include <cstdlib>
#include <iostream>

//----------------
//Actor
//----------------


Actor::Actor(StudentWorld* myWorld, int imageID, int startDirection, double startX, double startY, double size, int depth,  int speedX, int speedY, bool cAWA) : GraphObject(imageID, startX, startY, startDirection, size, depth), world(myWorld), alive(true), m_cAWA(cAWA), m_speedX(speedX), m_speedY(speedY), m_wasHit(false)
{
    
}

void Actor::moveActor(){
 
    int adj = getWorld()->getGhostRacer()->getSpeedY();
    int vert_speed = getSpeedY() - (adj);
    int horiz_speed = getSpeedX();
    
 
    int new_y = getY() + vert_speed;
    int new_x = getX() + horiz_speed;
    

    GraphObject::moveTo(new_x, new_y);

    
    
    if (new_x < 0 || new_x > VIEW_WIDTH || new_y < 0 || new_y > VIEW_HEIGHT){
        setAlive(false);
    }
    
}

bool Actor::Crash(Actor* comp) const
{
    double delta_x = fabs(comp->getX() - getX());
    double delta_y = fabs(comp->getY() - getY());
    double sumOfRadi = comp->getRadius() + getRadius();
    
    if (delta_x < (sumOfRadi * .25) && delta_y < (sumOfRadi * .6)) {
        return true;
    } else
        return false;
}


//----------------
//Character
//----------------
Character::Character(StudentWorld* myWorld, int imageID, int startDirection, double startX, double startY, double size, int depth, int forwardSpeedX, int forwardSpeedY, int health, bool cAWA) : Actor(myWorld, imageID, startDirection, startX, startY, size, depth, forwardSpeedX, forwardSpeedY, cAWA), m_health(health)
{
    
    
}



void Character::damage(int d){
   int newHealth = getHealth() - d;
    setHealth(newHealth);
    
    if (newHealth <= 0){
        setAlive(false);
    }
  
}

bool Actor::doesItHit(Actor* act1, Actor* act2)
{
    
    if (act1 == nullptr || act2 == nullptr){
        return false;
    }
    
    double delta_x = fabs(act1->getX() - act2->getX());
    double delta_y = fabs(act1->getY() - act2->getY());
    double sumOfRadi = act1->getRadius() + act2->getRadius();
    
    if (delta_x < (sumOfRadi * .25) && delta_y < (sumOfRadi * .6)) {
        act1->setWasHit(true);
        return true;
    } else
        return false;
}

//----------------
//GhostRacer
//----------------
GhostRacer::GhostRacer(StudentWorld* myWorld) : Character(myWorld, IID_GHOST_RACER, 90, 128, 32, 4.0, 0, 0,0, 100, true), max_shift_per_tick(4), m_water(10)
{
    
    
}



 void GhostRacer::doSomething()
{
    //1
    if (!isAlive()){
        return;
    }
    
    
    int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH / 2;
    int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH / 2;
    
    //2
    //check if gr is at a boarder
    if (getX() <= LEFT_EDGE){
        if (getDirection() > 90){
            damage(10);
            setDirection(82);
            getWorld()->playSound(SOUND_VEHICLE_CRASH);
           
        }
    }
    
    
    
    //3
    //check if gr is at a boarder
    if (getX() >= RIGHT_EDGE){
        if (getDirection() < 90){
            damage(10);
            setDirection(98);
            getWorld()->playSound(SOUND_VEHICLE_CRASH);
            
        }
    }
    
    //4
    int kp;
    if (getWorld()->getKey(kp))
    {
        switch (kp)
        {
            case KEY_PRESS_UP:
                if (getSpeedY() < 5)
                setSpeedY(getSpeedY() + 1);
                break;
            case KEY_PRESS_DOWN:
                if (getSpeedY() > -1)
                setSpeedY(getSpeedY() - 1);
                break;
            case KEY_PRESS_LEFT:
                if (getDirection() < 114)
                    setDirection(getDirection() + 8);
                break;
            case KEY_PRESS_RIGHT:
                if (getDirection() > 66)
                    setDirection(getDirection() - 8);
                break;
            case KEY_PRESS_SPACE:
                if (m_water > 0) {
                    double delta_x = SPRITE_HEIGHT * cos(getDirection() * 3.14 / 180);
                    double delta_y = SPRITE_HEIGHT * sin(getDirection() * 3.14 / 180);
                    getWorld()->addProjectile(getX() + delta_x, getY() + delta_y, getDirection());
                    
                   
                        getWorld()->playSound(SOUND_PLAYER_SPRAY);
                        m_water--;
                               }
                break;
        
                }
        
        
            }
        
    move();
    
    
    
    
}
void GhostRacer::OilSlick(){
    
   
     int r1 = randInt(5, 20);
     int r2 = randInt(0, 1);
    if (r2 == 0){
         r2 = -1;
    }
    else{
         r2 = 1;
    }
   
     setDirection(getDirection() + r1 * r2);
     
    
     int hold = 0;
     if (getDirection() < 60) {
         hold = getDirection() - 60;
         setDirection(getDirection() - hold);
     } else if (getDirection() > 120) {
         hold = getDirection() - 120;
         setDirection(getDirection() - hold);
     }
    
    
}


void GhostRacer::move(){
    double xx = (cos(getDirection() * 3.141592 / 180)) * max_shift_per_tick;
    moveTo(getX() + xx, getY());
    
}

//----------------
//Boarderlines
//----------------

BorderLine::BorderLine(StudentWorld* myWorld, int ID, int x, int y) : Actor(myWorld, ID, 0, x, y, 2, 2, 0, -4, false)

//it should already be set to alive cause its an actor
{
    
}


//----------------
//ZombieCab
//----------------

ZombieCab::ZombieCab(StudentWorld* myWorld, int x, int y, int speedX, int speedY) : Character(myWorld, IID_ZOMBIE_CAB, 90, x, y, 4, 0, speedX, speedY, 3, true), m_Hit(false), m_pl(0)
{
    
    
}


void ZombieCab::doSomething(){
    
    //1
    if (!isAlive()){
        return;
    }
    
    
    if (WasHit()){
        setWasHit(false);
        setHealth(getHealth()-1);
        if (getHealth() <= 0){
            setAlive(false);
            getWorld()->playSound(SOUND_VEHICLE_DIE);
            int r = randInt(1, 5);
            if (r == 1){
                getWorld()->addOil(getX(), getY());
            }
            getWorld()->increaseScore(200);
            return;
            
        }
        
        getWorld()->playSound(SOUND_VEHICLE_HURT);
        
    }
    
    //2
    if (Crash(getWorld()->getGhostRacer())){
       //a
        if (Hit() == true){
            ;
            //b
        }else{
            //c
            getWorld()->playSound(SOUND_VEHICLE_CRASH);
            //d
            getWorld()->getGhostRacer()->damage(20);
            //e
            if (getX() <= getWorld()->getGhostRacer()->getX()){
                setSpeedX(-5);
                setDirection(120 - randInt(0, 20));
            }
            //f
            if (getX() >= getWorld()->getGhostRacer()->getX()){
                setSpeedX(5);
                setDirection(60 - randInt(0, 20));
            }
        }
            
        setHit(true);
        
        }
    
    //3
    moveActor();
        
   
    //4
    if (getSpeedY() > getWorld()->getGhostRacer()->getSpeedY() && getWorld()->getLaneCharBottem() != nullptr){
        if (getWorld()->getLaneCharBottem()->getY()- 96 < getY()){
            setSpeedY(getSpeedY()- 0.5);
            return;
        }
        
        
    }
    
    //5
    if (getSpeedY() < getWorld()->getGhostRacer()->getSpeedY() && getWorld()->getLaneCharTop() != nullptr){
        if ( getWorld()->getLaneCharTop()->getY()+ 96 > getY()){
            setSpeedY(getSpeedY()+ 0.5);
            return;
        }
    }
    //6
    setplanLength(planLength() - 1);
    
    //7
    if (planLength() > 0){
        return;
        //8
    } else {
        setplanLength(randInt(4, 32));
        setSpeedY(getSpeedY() + randInt(-2, 2));
        
    }
    
    
    
    
    
    
}


//----------------
//Pedestrian
//----------------

Pedestrian::Pedestrian(StudentWorld* myWorld, int ID, int x, int y, int speedX, int speedY, int dir, int size ,  int depth, int health) : Character(myWorld, ID, dir, x, y, size, depth, speedX, speedY, health, true), m_plan(0)
{
    
    
}


void Pedestrian::setPlan()
{
  
    int rand = randInt(1, 3);
    int rand2 = randInt(0, 1);
    if (rand2 == 0)
        rand2 = -1;
    else
        rand2 = 1;
    
  
    int horiz_speed = rand * rand2;
    
    setSpeedX(horiz_speed);
    
   
    m_plan = randInt(4, 32);

    if (horiz_speed < 0)
        setDirection(180);
    else
        setDirection(0);
}
//----------------
//HumanPedestrian
//----------------

HumanPedestrian::HumanPedestrian(StudentWorld* myWorld, int x, int y) : Pedestrian(myWorld, IID_HUMAN_PED, x, y, 0, -4, 0, 2, 0, 2)
{
    
    
}

void HumanPedestrian::doSomething()
{
    if (!isAlive())
        return;

   
    if (Crash(getWorld()->getGhostRacer())) {
        getWorld()->getGhostRacer()->setAlive(false);
        return;
    }

   
   moveActor();
    if (!isAlive())
        return;

   
    decPlan();

   
    if (getPlan() > 0)
        return;
    else {
        setPlan();
    }
}

//----------------
//ZombiePedestrian
//----------------

ZombiePedestrian::ZombiePedestrian(StudentWorld* myWorld, int x, int y) : Pedestrian(myWorld, IID_ZOMBIE_PED, x, y, 0, -4, 0, 3, 0, 2), m_ticks(0)
{
    
    
}

void ZombiePedestrian::doSomething()
{
    if (!isAlive())
    return;
    
    //check if ghostracer went over a ped
    if (Crash(getWorld()->getGhostRacer())) {
        getWorld()->getGhostRacer()->damage(5);
      
        
        setAlive(false);
        getWorld()->playSound(SOUND_PED_DIE);
    }
    
    if (WasHit()){
        setWasHit(false);
        setHealth(getHealth()-1);
        if (getHealth() <= 0){
            setAlive(false);
            getWorld()->playSound(SOUND_PED_DIE);
            int r = randInt(1, 5);
            if (r == 1){
                getWorld()->addHealGoodie(getX(), getY());
                getWorld()->increaseScore(150);
            }
            getWorld()->playSound(SOUND_PED_DIE);
        }
        
        
        
        
        
        
    }
    
    
    
   
    double dx =  getWorld()->getGhostRacer()->getX() - getX();
       double adx = fabs(dx);
       
      
       if (adx <= 30 && getWorld()->getGhostRacer()->getY() < getY()) {
           setDirection(270);
           
           //change speed depending on if zombieped is one left, right, or directly in front
           if (dx > 0){
               setSpeedX(1);
           }
           else if (dx < 0){
               setSpeedX(-1);
           }
           else{
               setSpeedX(0);
           }
       
       
      
       m_ticks--;
       
    
       if (m_ticks <= 0) {
           getWorld()->playSound(SOUND_ZOMBIE_ATTACK);
           m_ticks = 20;
       }

       }
   
    moveActor();
    if (!isAlive())
        return;
    
  
    if (getPlan() > 0) {
        decPlan();
        return;
    }
    else {
        setPlan();
    }
}


//----------------
//OilSlick
//----------------

OilSlick::OilSlick(StudentWorld* myWorld, int x, int y) : Actor(myWorld, IID_OIL_SLICK, 0, x, y, randInt(2, 5), 2, 0, -4, false)
{
    
    
}


void OilSlick::doSomething()
{
    if (!isAlive())
    return;
    
    moveActor();
    if (!isAlive())
        return;
    
    if (Crash(getWorld()->getGhostRacer())) {
        getWorld()->playSound(SOUND_OIL_SLICK);
       getWorld()->getGhostRacer()->OilSlick();
    }
}


//----------------
//HealingGoodie
//----------------

HealingGoodie:: HealingGoodie(StudentWorld* myWorld, int x, int y) : Actor(myWorld, IID_HEAL_GOODIE, 0, x, y, 1, 2, 0, -4, false)
{
    
    
}

void HealingGoodie::doSomething()
{
    if (!isAlive())
    return;
    
    moveActor();
    if (!isAlive())
        return;
    
    if (Crash(getWorld()->getGhostRacer())) {
        getWorld()->getGhostRacer()->damage(-10);
        setAlive(false);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(250);
    }
}

//----------------
//Soul Goodie
//----------------

SoulGoodie::SoulGoodie(StudentWorld* myWorld, int x, int y) : Actor(myWorld, IID_SOUL_GOODIE, 0, x, y, 4, 2, 0, -4, false)
{
    
    
}

void SoulGoodie::doSomething(){
    
    if (!isAlive())
    return;
    
    moveActor();
    if (!isAlive())
        return;
    
    if (Crash(getWorld()->getGhostRacer())) {
        getWorld()->incSouls();
        setAlive(false);
        getWorld()->playSound(SOUND_GOT_SOUL);
        getWorld()->increaseScore(100);
    }
    setDirection(getDirection()-10);
    
}
//----------------
//HolyWaterGoodie
//----------------

HolyWaterGoodie::HolyWaterGoodie(StudentWorld* myWorld, int x, int y) :
Actor(myWorld, IID_HOLY_WATER_GOODIE, 90, x, y, 2, 2, 0, -4, false)
{
    
    
}

void HolyWaterGoodie::doSomething(){
    if (!isAlive())
    return;
    
    moveActor();
    if (!isAlive())
        return;
    
    if (Crash(getWorld()->getGhostRacer())) {
        getWorld()->getGhostRacer()->setHolyWater(getWorld()->getGhostRacer()->holyWater() + 10);
        setAlive(false);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(50);
    }
    
}

//----------------
//HolyWaterProjectile
//----------------


HolyWaterProjectile:: HolyWaterProjectile(StudentWorld* myWorld, int x, int y, int speedX, int speedY, int dir) : Actor(myWorld, IID_HOLY_WATER_PROJECTILE, dir, x, y, 1, 1, speedX, speedY, false), m_shot(false), m_distance(0), m_maxLength(160)
{
    
    
}

void HolyWaterProjectile::doSomething()
{
    if (!isAlive())      
    return;
    
    
    if (getWorld()->damgeTakers(this)) {
        //attempt damaging the other object
        setAlive(false);
        return;
    }
     
   
    moveForward(SPRITE_HEIGHT);
    
   
    if (!isAlive())
        return;
    
   
    m_distance += SPRITE_HEIGHT;
    
  
    if (m_distance >= m_maxLength)
        setAlive(false);
}
