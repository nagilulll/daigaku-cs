#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;
class Agent;
 
class Actor : public GraphObject 
{
public:
    Actor(StudentWorld* world, int startX, int startY, int imageID, int hitPoints, int startDir);
    virtual void doSomething() = 0;
    bool isAlive() const;
    void setDead();
    StudentWorld* getWorld() const;
    
      // Reduce this actor's hit points
    void decHitPoints(int amt);
    
      // Can an agent occupy the same square as this actor?
    virtual bool allowsAgentColocation() const;
    
      // Can a marble occupy the same square as this actor?
    virtual bool allowsMarble() const;
    
      // Does this actor count when a factory counts items near it?
    virtual bool countsInFactoryCensus() const;
    
      // Does this actor stop peas from continuing?
    virtual bool stopsPea() const;
    
      // Can this actor be damaged by peas?
    virtual bool isDamageable() const;

      // Cause this Actor to sustain damageAmt hit points of damage.
    virtual void damage(int damageAmt);
    
      // Can this actor be pushed by a to location x,y?
    virtual bool bePushedBy(Agent* a, int x, int y);
    
      // Can this actor be swallowed by a pit?
    virtual bool isSwallowable() const;
    
      // Can this actor be picked up by a ThiefBot?
    virtual bool isStealable() const;
    
      // How many hit points does this actor have left?
    virtual int getHitPoints() const;
    
      // Set this actor's hit points to amt.
    virtual void setHitPoints(int amt);
    
      // Make the actor sustain damage.  Return true if this kills the
      // actor, and false otherwise.
    virtual bool tryToBeKilled(int damageAmt);

private:
    int m_hitPoints;
    bool m_alive;
    StudentWorld* m_world;
};

class Agent : public Actor
{
public:
    Agent(StudentWorld* world, int startX, int startY, int imageID, int hitPoints, int startDir);

      // Move to the adjacent square in the direction the agent is facing
      // if it is not blocked, and return true.  Return false if the agent
      // can't move.
    bool moveIfPossible();

      // Return true if this agent can push marbles (which means it's the
      // player).
    virtual bool canPushMarbles() const;

      // Return true if this agent doesn't shoot unless there's an unobstructed
      // path to the player.
    virtual bool needsClearShot() const;

      // Return the sound effect ID for a shot from this agent.
    virtual int shootingSound() const;
};


class Avatar : public Agent
{
public:
    Avatar(StudentWorld* world, int startX, int startY);
    virtual void doSomething();
    int Ammo() const;
    void updateAmmo(int peas);
    void resetHealth();
    int getHealthPercent() const;
    virtual bool isDamageable() const;
    virtual void damage(int damageAmt);
    virtual bool canPushMarbles() const;
    virtual bool needsClearShot() const;
    virtual int shootingSound() const;
    void goodieLife(int life);
    void goodieHealth(int health);
    void goodiePeas(int peas);

private:
    int m_peas;
    void move(int dir);
    void shoot();
};


class Robot : public Agent
{
public:
    Robot(StudentWorld* world, int startX, int startY, int imageID, int hitPoints, int score, int startDir);
    virtual void doSomething() const;
    virtual bool isDamageable() const;
    virtual void damage(int damageAmt);
    virtual bool canPushMarbles() const;
    virtual bool needsClearShot() const;
    virtual int shootingSound() const;

      // Does this robot shoot?
    virtual bool isShootingRobot() const;
    
private:
    int m_score;
};


class RageBot : public Robot
{
public:
    RageBot(StudentWorld* world, int startX, int startY, int startDir);
    virtual void doSomething();
    int reverseDirection(int dir) const;
};

class ThiefBot : public Robot
{
public:
    ThiefBot(StudentWorld* world, int startX, int startY, int imageID,
                         int hitPoints, int score);
    virtual void doSomething();
    virtual bool countsInFactoryCensus() const;
    virtual void damage(int damageAmt);
};

class RegularThiefBot : public ThiefBot
{
public:
    RegularThiefBot(StudentWorld* world, int startX, int startY);
    virtual void doSomething();
    virtual bool isShootingRobot() const;
};

class MeanThiefBot : public ThiefBot
{
public:
    MeanThiefBot(StudentWorld* world, int startX, int startY);
    virtual void doSomething();
};

class Exit : public Actor
{
public:
    Exit(StudentWorld* world, int startX, int startY);
    virtual void doSomething();
    virtual bool allowsAgentColocation() const;
};

class Wall : public Actor
{
public:
    Wall(StudentWorld* world, int startX, int startY);
    virtual void doSomething();
    virtual bool stopsPea() const;
    
private:
    
};

class Marble : public Actor
{
public:
    Marble(StudentWorld* world, int startX, int startY);
    virtual void doSomething();
    virtual bool isDamageable() const;
    virtual void damage(int damageAmt);
    virtual bool isSwallowable() const;
    virtual bool bePushedBy(Agent* a, int x, int y);
};

class Pit : public Actor
{
public:
    Pit(StudentWorld* world, int startX, int startY);
    virtual void doSomething();
    virtual bool allowsMarble() const;
};

class Pea : public Actor
{
public:
    Pea(StudentWorld* world, int startX, int startY, int startDir);
    virtual void doSomething();
    virtual bool allowsAgentColocation() const;
};

class ThiefBotFactory : public Actor
{
public:
    enum ProductType { REGULAR, MEAN };

    ThiefBotFactory(StudentWorld* world, int startX, int startY, ProductType type);
    virtual void doSomething();
    virtual bool stopsPea() const;

private:
    ProductType m_productType;
};

class PickupableItem : public Actor
{
public:
    PickupableItem(StudentWorld* world, int startX, int startY, int imageID, int score);
    virtual void doSomething();
    virtual bool allowsAgentColocation() const;
    
private:
    int m_score;
};

class Crystal : public PickupableItem
{
public:
    Crystal(StudentWorld* world, int startX, int startY);
};

class Goodie : public PickupableItem
{
public:
    Goodie(StudentWorld* world, int startX, int startY, int imageID, int score);
    virtual void doSomething();
    virtual bool isStealable() const;

      // Set whether this goodie is currently stolen.
    void setStolen(bool status);
private:
    bool m_stolen;
    virtual void goodieBuff() = 0;
};

class ExtraLifeGoodie : public Goodie
{
public:
    ExtraLifeGoodie(StudentWorld* world, int startX, int startY);
private:
    virtual void goodieBuff();
};

class RestoreHealthGoodie : public Goodie
{
public:
    RestoreHealthGoodie(StudentWorld* world, int startX, int startY);
private:
    virtual void goodieBuff();
};

class AmmoGoodie : public Goodie
{
public:
    AmmoGoodie(StudentWorld* world, int startX, int startY);
private:
    virtual void goodieBuff();
};





#endif // ACTOR_H_
