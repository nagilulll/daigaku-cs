#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;
 
class Actor : public GraphObject 
{
public:
    Actor(int imageId, double startX, double startY, int dir, double size, StudentWorld* world);
    virtual void doSomething() = 0;
    int getHealth();
    void updateHealth(int health);
    StudentWorld* getWorld();

private:
    int m_health;
    StudentWorld* m_world;
};

class Wall : public Actor 
{
public:
    Wall(double startX, double startY, StudentWorld* world);
    virtual void doSomething();
    
private:
    
};

class Avatar : public Actor 
{
public:
    Avatar(double startX, double startY, StudentWorld* world);
    virtual void doSomething();
    int updateAmmo(int peas)
    {
        return m_peas;
    }

private:
    int m_peas;
    void move(int dir);
};

#endif // ACTOR_H_
