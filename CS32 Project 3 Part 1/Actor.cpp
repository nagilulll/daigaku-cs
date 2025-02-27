#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(int imageId, double startX, double startY, int dir, double size, StudentWorld* world) : GraphObject(imageId, startX, startY, dir, size), m_world(world)
{
}

int Actor::getHealth()
{
    return m_health;
}

void Actor::updateHealth(int health)
{
    m_health = health;
}

StudentWorld* Actor::getWorld() 
{
    return m_world;
}

Wall::Wall(double startX, double startY, StudentWorld* world) : Actor(IID_WALL, startX, startY, 0, 1, world)
{
    setVisible(true);
}

void Wall::doSomething()
{
}

Avatar::Avatar(double startX, double startY, StudentWorld* world) : Actor(IID_PLAYER, startX, startY, 0, 1, world)
{
    updateHealth(20);
    updateAmmo(20);
    setDirection(right);
    setVisible(true);
}

void Avatar::doSomething()
{
    int ch;
    if (getWorld()->getKey(ch))
    {
        switch (ch)
        {
            case KEY_PRESS_UP:
                move(up);
                break;
            case KEY_PRESS_DOWN:
                move(down);
                break;
            case KEY_PRESS_LEFT:
                move(left);
                break;
            case KEY_PRESS_RIGHT:
                move(right);
                break;
            /*case KEY_PRESS_SPACE:
                shoot();
                break;
            case KEY_PRESS_ESCAPE:
                updateHealth(0);
                break;*/
        }
    }
}

void Avatar::move(int dir)
{
    double currentX = getX();
    double currentY = getY();
    
    switch (dir)
    {
        case left:
            currentX--;
            break;
        case right:
            currentX++;
            break;
        case up:
            currentY++;
            break;
        case down:
            currentY--;
            break;
    }
    
    Actor* actors = nullptr;
    if (!getWorld()->occupy(currentX, currentY, actors))
    {
        setDirection(dir);
        moveTo(currentX, currentY);
    }
}



/*void Avatar::shoot()
{
    
}*/
