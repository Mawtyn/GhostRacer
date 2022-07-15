#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;

class Actor : public GraphObject
{
public:
    Actor(StudentWorld* myWorld, int imageID, int startDirection, double startX, double startY, double size, int depth, int speedX, int speedY, bool cAWA);
    virtual ~Actor(){};
    virtual void doSomething() = 0; //actor does nothing
    void moveActor();
    bool isAlive(){return alive;};
    void setAlive(bool a){alive = a;};
    bool getcAWA(){return m_cAWA;};
    bool Crash(Actor* comp) const;
    int getSpeedX(){return m_speedX;};
    void setSpeedX(int x){m_speedX = x;};
    int getSpeedY(){return m_speedY;};
    void setSpeedY(int y){m_speedY = y;};
    virtual bool canGetShot (){return false;};
    bool doesItHit(Actor* act1, Actor* act2);
    bool WasHit() {return m_wasHit;};
    void setWasHit(bool a){m_wasHit = a;};
    StudentWorld* getWorld() const {return world;};
    
private:
    StudentWorld* world;
    bool alive;
    bool m_cAWA;
    int m_speedX;
    int m_speedY;
    bool m_wasHit;

};



class Character : public Actor
{
public:
    Character(StudentWorld* myWorld, int imageID, int startDirection, double startX, double startY, double size, int depth, int forwardSpeedX, int forwardSpeedY, int health, bool cAWA);
    virtual ~Character(){};
    virtual void doSomething() = 0;
    int getHealth(){return m_health;};
    void setHealth(int x){m_health = x;};
    void damage(int d);
  
    
private:


    int m_health;
};

class GhostRacer : public Character
{
public:
    GhostRacer(StudentWorld* myWorld);
    virtual ~GhostRacer(){};
    void move();
    void OilSlick();
    virtual void doSomething();
    int holyWater() {return m_water;};
    void setHolyWater(int x) {m_water = x;};

private:
    int m_water;
//    int m_dir;
        int max_shift_per_tick;
};


class BorderLine : public Actor
{
public:
    BorderLine(StudentWorld* myWorld, int ID, int x, int y);
    virtual ~BorderLine(){};
    virtual void doSomething(){moveActor();};

};



class ZombieCab : public Character
{
public:
    ZombieCab(StudentWorld* myWorld, int x, int y, int speedX, int speedY);
    virtual ~ZombieCab(){};
    virtual void doSomething();
    bool Hit(){return m_Hit;};
    void setHit(bool a){m_Hit = a;};
    int planLength(){return m_pl;};
    void setplanLength(int z){m_pl = z;};
   
    virtual bool canGetShot (){return true;};
private:
    bool m_Hit;
    int m_pl;
   

};

class Pedestrian: public Character      
{
public:
    Pedestrian(StudentWorld* myWorld, int ID, int x, int y, int speedX, int speedY, int dir, int size ,  int depth, int health);
    virtual ~Pedestrian() {};
    
    int getPlan() const {return m_plan;};
    void decPlan() {m_plan--;};
    void setPlan();
    
    
private:
    int m_plan;
};

class HumanPedestrian: public Pedestrian
{
public:
    HumanPedestrian(StudentWorld* myWorld, int x, int y);
    virtual ~HumanPedestrian(){};
    virtual void doSomething ();
    virtual bool canGetShot (){return true;};
    
private:
  
};

class ZombiePedestrian: public Pedestrian
{
public:
    ZombiePedestrian(StudentWorld* myWorld, int x, int y);
    virtual ~ZombiePedestrian(){};
    virtual void doSomething ();
    virtual bool canGetShot (){return true;};
   
private:
    int m_ticks;
   
};

class OilSlick : public Actor
{
public:
    OilSlick(StudentWorld* myWorld, int x, int y);
    virtual ~OilSlick(){};
    virtual void doSomething ();
};

class HealingGoodie : public Actor
{
public:
   HealingGoodie(StudentWorld* myWorld, int x, int y);
    virtual ~HealingGoodie(){};
    virtual void doSomething ();
};

class SoulGoodie : public Actor
{
public:
    SoulGoodie(StudentWorld* myWorld, int x, int y);
    virtual ~SoulGoodie(){};
    virtual void doSomething ();
};

class HolyWaterGoodie : public Actor
{
public:
    HolyWaterGoodie(StudentWorld* myWorld, int x, int y);
    virtual ~HolyWaterGoodie(){};
    virtual void doSomething();
};

class HolyWaterProjectile: public Actor
{
public:
    HolyWaterProjectile(StudentWorld* myWorld, int x, int y, int speedX, int speedY, int dir);
    virtual ~HolyWaterProjectile() {};
    virtual void doSomething();
    bool shot() {return m_shot;};
    void setShot(bool a){m_shot = a;};
  
private:
    int m_maxLength;
    int m_distance;
    bool m_shot;
   
};

#endif // ACTOR_H_
