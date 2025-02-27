#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
using namespace std;

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

int StudentWorld::init()
{
    string curLevel = "";
    if (getLevel() < 10)
        curLevel = "level0" + to_string(getLevel()) + ".txt";
    else if (getLevel() >= 10)
        curLevel = "level" + to_string(getLevel()) + ".txt";
    Level lev(assetPath());
    Level::LoadResult result = lev.loadLevel(curLevel);
    if (result == Level::load_fail_file_not_found ||
     result == Level:: load_fail_bad_format)
     return -1;
    for (int x = 0; x < VIEW_WIDTH; x++)
    {
        for (int y = 0; y < VIEW_HEIGHT; y++)
        {
            Level::MazeEntry item = lev.getContentsOf(x, y);
            if (item == Level::player)
                m_avatar = new Avatar(x, y, this);
            else if (item == Level::wall)
                m_actors.push_back(new Wall(x, y, this));
            else
                continue;
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you type q

    setGameStatText("Game will end when you type q");
    
    m_avatar->doSomething();
    
    for (list<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); ++it) {
        (*it)->doSomething();
    }
    
	return GWSTATUS_CONTINUE_GAME;
}


void StudentWorld::cleanUp()
{
    delete m_avatar;
    m_avatar = nullptr;

    for (list<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); ++it)
    {
        delete *it;
    }
    m_actors.clear();
}

StudentWorld::~StudentWorld() 
{
    cleanUp();
}

bool StudentWorld::occupy(double x, double y, Actor*& object) 
{
    for (list<Actor*>::iterator actor = m_actors.begin(); actor != m_actors.end(); ++actor)
    {
        double actorX, actorY;
        actorX = (*actor)->getX();
        actorY = (*actor)->getY();

        if (actorX == x && actorY == y)
        {
            object = *actor;
            return true;
        }
    }
    return false;
}
