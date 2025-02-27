#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

#include "Actor.h"
#include <list>
using namespace std;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    bool occupy(double x, double y, Actor*& object);
    
private:
    list<Actor*> m_actors;
    Avatar* m_avatar;
};


#endif // STUDENTWORLD_H_
