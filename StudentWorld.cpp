#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}


StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
, m_laneCharBottem(nullptr), m_laneCharTop(nullptr), m_souls(0), m_bonus(5000)

{
}

int StudentWorld::souls2save(){
    return  (2 * getLevel() +5);
}

bool StudentWorld::damgeTakers(Actor* actor)
{
   
    vector<Actor*>::iterator it;
    for (it = m_actors.begin(); it != m_actors.end(); it++) {
        if ((*it)->canGetShot() && getGhostRacer()->doesItHit(*it, actor)) {
            return true;
        }
    }
    
    return false;
}

void StudentWorld::addProjectile(int x, int y, int dir){
    
    m_actors.push_back(m_holyWaterProjectile = new HolyWaterProjectile(this, x, y, 0, 0, dir));
    
}

int StudentWorld::init()
{
    m_lastC = 0;
    m_actors.push_back(m_ghostRacer = new GhostRacer(this));
 
    
    
    
    
    
        int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH / 2;
        int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH / 2;
        int num = VIEW_HEIGHT / SPRITE_HEIGHT;
        for (int i = 0; i < num; i++){
            m_actors.push_back(new BorderLine(this, IID_YELLOW_BORDER_LINE,RIGHT_EDGE, i * SPRITE_HEIGHT));
    //        new BorderLine(this, IID_YELLOW_BORDER_LINE,RIGHT_EDGE, i * SPRITE_HEIGHT);
            m_actors.push_back(new BorderLine(this, IID_YELLOW_BORDER_LINE, LEFT_EDGE, i * SPRITE_HEIGHT));

        }
    
        int s = (VIEW_HEIGHT / (4 * SPRITE_HEIGHT));
        for (int i = 0; i < s; i++){
    m_actors.push_back(new BorderLine(this, IID_WHITE_BORDER_LINE, RIGHT_EDGE - ROAD_WIDTH / 3, i * (SPRITE_HEIGHT * 4)));
    m_actors.push_back(new BorderLine(this, IID_WHITE_BORDER_LINE, LEFT_EDGE + ROAD_WIDTH / 3, i * (SPRITE_HEIGHT * 4)));
    
    
    
        }
    
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
   
    ostringstream text;
    text << "Score: " << getScore() << "  ";
    text << "Lvl: " << getLevel() << "  ";
    text << "Souls2Save: " << souls2save() - m_souls << "  ";
    text << "Lives: " << getLives() << "  ";
    text << "Health: " << getGhostRacer()->getHealth() << "  ";
    text << "Sprays: " << getGhostRacer()->holyWater() << "  ";
    text << "Bonus: " << m_bonus << "  ";
    
    setGameStatText(text.str());

    
    vector<Actor*>::iterator it;
    for (it = m_actors.begin(); it != m_actors.end(); it++){

        (*it)->doSomething();
       
        if (getGhostRacer()->isAlive() == false){
            m_souls = 0;
            decLives();
            playSound(SOUND_PLAYER_DIE);
            return GWSTATUS_PLAYER_DIED;
            
        }
    }
    

    
    
   

    
    for (it = m_actors.begin(); it != m_actors.end();){
        if (!(*it)->isAlive()){
        delete *it;
            it = m_actors.erase(it);
        } else {
            it++;
        }
    }
    
    
    
    int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH / 2;
    int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH / 2;
    
    double whiteRight = RIGHT_EDGE - ROAD_WIDTH / 3;
    double whiteLeft = LEFT_EDGE + ROAD_WIDTH / 3;
    
    double new_border_y = VIEW_HEIGHT - SPRITE_HEIGHT;
//    double last = (VIEW_HEIGHT / (4 * SPRITE_HEIGHT) - 1) * 4 * SPRITE_HEIGHT;
    double last = m_lastC;
    double delta_y = new_border_y - last;
    
    for (double i = 0; i < delta_y; i++){
        if (delta_y >= SPRITE_HEIGHT){
        m_actors.push_back(new BorderLine(this, IID_YELLOW_BORDER_LINE,RIGHT_EDGE, new_border_y));
            m_actors.push_back(new BorderLine(this, IID_YELLOW_BORDER_LINE,LEFT_EDGE, new_border_y));
            
           
        }
        
        
        if (delta_y >= (4 * SPRITE_HEIGHT)){
            m_actors.push_back(new BorderLine(this, IID_WHITE_BORDER_LINE,whiteRight, new_border_y));
            m_actors.push_back(new BorderLine(this, IID_WHITE_BORDER_LINE,whiteLeft, new_border_y));
            m_lastC = new_border_y;
            m_lastC += - 4 - m_ghostRacer->getSpeedY();
            
            
        }
    }
        m_lastC += -4 - m_ghostRacer->getSpeedY();
      
    
    
    
    
   
    
    
    //----------------
    //ZombieCar
    //----------------
  
    int L = getLevel();
    int ChanceVehicle = max(100 - L * 10, 20);
    int cV = randInt(0, ChanceVehicle);
    if (cV == 0){
        
        
        
//1
        int leftLane;
        int rightLane;
       
        int cur_lane = randInt(1, 3);
//2
        for (int i = 0; i < 3; i++){
        if (cur_lane == 1){
            //left lane
            leftLane = LEFT_EDGE;
            rightLane = whiteLeft;
        
        } else if (cur_lane == 2) {
            //middle lane
            leftLane = whiteLeft;
            rightLane = whiteRight;
        } else {
            //right lane
            leftLane = whiteRight;
            rightLane = RIGHT_EDGE;
            
        }
        int middleOfLaneX = ((rightLane - leftLane)/2) + leftLane;
        

        laneActor(leftLane, rightLane);
        if (m_laneCharBottem == nullptr || m_laneCharBottem->getY() > VIEW_HEIGHT/3){
            m_actors.push_back(m_zombieCab = new ZombieCab(this, middleOfLaneX, SPRITE_HEIGHT /2, 0, getGhostRacer()->getSpeedY() + randInt(2, 4)));
            break;
        } else if (m_laneCharTop == nullptr || m_laneCharTop->getY() < VIEW_HEIGHT* 2/3){
            m_actors.push_back(m_zombieCab = new ZombieCab(this, middleOfLaneX, VIEW_HEIGHT - SPRITE_HEIGHT /2, 0, getGhostRacer()->getSpeedY() - randInt(2, 4)));
            
            break;
            
        }
            //runs through all the lanes just in case one doesnt work
            if (cur_lane == 3){
                cur_lane = 1;
            } else {
                cur_lane++;
            }
            
            
            
        }
        
        
//3
//4
//5
//6 did them already
    }
    
    //----------------
    //Oil SLicks
    //----------------
    int ChanceOilSlick = max(150 - L * 10, 40);
    int cOS = randInt(0, ChanceOilSlick);
    if (cOS == 0){
        
        m_actors.push_back(m_oilslick = new OilSlick(this, randInt(LEFT_EDGE, RIGHT_EDGE), VIEW_HEIGHT));
    }
    
    //----------------
    //ZombiePeds
    //----------------
    int ChanceZombiePed = max(100 - L * 10, 20);
    int cZP = randInt(0, ChanceZombiePed);
    if (cZP == 0){
        
        m_actors.push_back(m_zombiePed = new ZombiePedestrian(this, randInt(0, VIEW_WIDTH), VIEW_HEIGHT));
    }
    
    //----------------
    //HumanPeds
    //----------------
    
    
    int ChanceHumanPed = max(200 - L * 10, 30);
    int cHP = randInt(0, ChanceHumanPed);
    if (cHP == 0){
        
        m_actors.push_back(m_humanPed = new HumanPedestrian(this, randInt(0, VIEW_WIDTH), VIEW_HEIGHT));
    }
    //----------------
    //Holywater
    //----------------
    int ChanceHR = 100 + 10 * L;
    int cHR = randInt(0, ChanceHR);
    if (cHR == 0){
        
        m_actors.push_back(m_holyWaterGoodie = new HolyWaterGoodie(this,randInt(LEFT_EDGE, RIGHT_EDGE), VIEW_HEIGHT));
    }
    
    //----------------
    //Lost soul
    //----------------
    
    int ChanceSoul = 100;
    int cS = randInt(0, ChanceSoul);
    if (cS == 0){
        
        m_actors.push_back(m_soulGoodie = new SoulGoodie(this,randInt(LEFT_EDGE, RIGHT_EDGE), VIEW_HEIGHT));
    }
    

    
   
    if (souls2save() - m_souls == 0){
        m_souls = 0;
        increaseScore(m_bonus);
        m_bonus = 5000;
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL ;
    }
    
   
    m_bonus--;
    
    
    
    return GWSTATUS_CONTINUE_GAME;
    

}

void StudentWorld::cleanUp()
{
   
    vector<Actor*>::iterator it;
    for (it = m_actors.begin(); it != m_actors.end();){
        delete *it;
            it = m_actors.erase(it);
    }
}



void StudentWorld::addHealGoodie(int x, int y){
    
    m_actors.push_back(m_healingGoodie = new HealingGoodie(this, x, y));
}

void StudentWorld::addOil(int x, int y){
    
    m_actors.push_back(m_oilslick = new OilSlick(this, x, y));
}

//checks if there is an actor with cAWA
bool StudentWorld::laneActor(int x, int x2){
    vector<Actor*>::iterator it;
    bool atLeast1 = false;
    for (it = m_actors.begin(); it != m_actors.end();it++){
    //this checks of it is a worth actor
    if ((*it)->getcAWA()){
        //is the object between the lanes
        if ((*it)->getX() > x && (*it)->getX() <= x2) {
            atLeast1 = true;
            
            if (m_laneCharBottem == nullptr){
                m_laneCharBottem = (*it);
            }
            if (m_laneCharTop == nullptr){
                m_laneCharTop = (*it);
            }
            
            
            
            //finds the lowwest actor and keeps it
            if ((*it)->getY() < m_laneCharBottem->getY()){
                m_laneCharBottem = (*it);
            }
            
            //finds the heights actor and keeps it
            if ((*it)->getY() > m_laneCharTop->getY()){
                m_laneCharTop = (*it);
            }
         
           
            
            
        }
    
        }
    }
    
    if (atLeast1 == true){
        return true;
    } else
    {
        m_laneCharTop = nullptr;
        m_laneCharBottem = nullptr;
    }
    //there is no actor on this lane
    return false;
}

