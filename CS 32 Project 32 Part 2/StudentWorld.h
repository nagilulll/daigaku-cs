#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"


// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

#include "Actor.h"
#include <list>
class Actor;
class Agent;
class Player;

// Note:  A convention used in a number of interfaces is to represent a
// direction with the adjustments to x and y needed to move one step in
// that direction:
// left:   dx is -1, dy is  0
// right:  dx is  1, dy is  0
// up:     dx is  0, dy is  1
// down:   dx is  0, dy is -1


class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    bool occupy(double x, double y, Actor*& object);
    bool checkMarbles(int x, int y);
    
    bool canAgentMoveTo(Agent* agent, int x, int y, int dx, int dy) const;
    bool canMarbleMoveTo(int x, int y) const;
    bool isPlayerColocatedWith(int x, int y) const;
    bool damageSomething(Actor* a, int damageAmt);
    bool swallowSwallowable(Actor* a);
    bool existsClearShotToPlayer(int x, int y, int dx, int dy) const;
    bool doFactoryCensus(int x, int y, int distance, int& count) const;
    Actor* getColocatedStealable(int x, int y) const;
    void restorePlayerHealth();
    void increaseAmmo();
    bool anyCrystals() const;
    bool decCrystals() const;
    void setLevelFinished();
    void addActor(Actor* a);
    void crystalCount(int count);
    
    void extraLife();
    void healthReset();
    void morePeas();
    
private:
    std::list<Actor*> m_actors;
    Avatar* m_avatar;
    int m_crystals;
    bool m_clear;
};


#endif // STUDENTWORLD_H_

