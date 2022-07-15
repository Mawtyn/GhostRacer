#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_
#include "Actor.h"
#include "GameWorld.h"
#include <string>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class GhostRacer;
class BorderLine;
class ZombieCab;
class HumanPedestrian;
class ZombiePedestrian;
class OilSlick;
class HealingGoodie;
class SoulGoodie;
class HolyWaterGoodie;
class HolyWaterProjectile;


class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    
    virtual ~StudentWorld(){cleanUp();}

    virtual int init();
    virtual int move();
    virtual void cleanUp();
    bool laneActor(int x, int x2);
    void addHealGoodie(int x, int y);
    void addOil(int x, int y);
    void addProjectile(int x, int y, int dir);
    int getSouls(){return m_souls;};
    void incSouls(){m_souls++;};
    int souls2save();
    bool damgeTakers(Actor* actor);
    

    GhostRacer* getGhostRacer() {return m_ghostRacer;};
    BorderLine* getBorderLine() {return m_borderLine;};
    Actor* getLaneCharBottem() {return m_laneCharBottem;};
    Actor* getLaneCharTop() {return m_laneCharTop;};
    
    


private:
    
    //allowing access to all the games actors through pointers
    std::vector<Actor*> m_actors;
    std::vector<Character*> m_characters;
    GhostRacer* m_ghostRacer;
    BorderLine* m_borderLine;
    BorderLine* m_borderLine2;
    ZombieCab* m_zombieCab;
    ZombiePedestrian* m_zombiePed;
    HumanPedestrian* m_humanPed;
    OilSlick* m_oilslick;
    HealingGoodie* m_healingGoodie;
    SoulGoodie* m_soulGoodie;
    HolyWaterGoodie* m_holyWaterGoodie;
    HolyWaterProjectile* m_holyWaterProjectile;
    
    double m_lastC;
    int m_souls;
    int m_bonus;

    Actor* m_laneCharBottem;
    Actor* m_laneCharTop;

    
    
};

#endif // STUDENTWORLD_H_
