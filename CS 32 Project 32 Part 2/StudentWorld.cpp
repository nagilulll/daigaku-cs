#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
using namespace std;

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

#include <sstream>

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
    else
        return GWSTATUS_LEVEL_ERROR;
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
                m_avatar = new Avatar(this, x, y);
            else if (item == Level::wall)
            {
                Wall* wall = new Wall(this, x, y);
                addActor(wall);
            }
            else if (item == Level::horiz_ragebot)
            {
                RageBot* horizrageBot = new RageBot(this, x, y, 90);
                addActor(horizrageBot);
            }
            else if (item == Level::vert_ragebot)
            {
                RageBot* vertrageBot = new RageBot(this, x, y, 180);
                addActor(vertrageBot);
            }
            else if (item == Level::exit)
            {
                Exit* exit = new Exit(this, x, y);
                addActor(exit);
            }
            else if (item == Level::marble)
            {
                Marble* marble = new Marble(this, x, y);
                addActor(marble);
            }
            else if (item == Level::crystal)
            {
                Crystal* crystal = new Crystal(this, x, y);
                addActor(crystal);
            }
            else if (item == Level::pit)
            {
                Pit* pit = new Pit(this, x, y);
                addActor(pit);
            }
            else if (item == Level::restore_health)
            {
                RestoreHealthGoodie* healthGoodie = new RestoreHealthGoodie(this, x, y);
                addActor(healthGoodie);
            }
            else if (item == Level::extra_life)
            {
                ExtraLifeGoodie* extraLife = new ExtraLifeGoodie(this, x, y);
                addActor(extraLife);
            }
            else if (item == Level::ammo)
            {
                AmmoGoodie* ammo = new AmmoGoodie(this, x, y);
                addActor(ammo);
            }
            else if (item == Level::thiefbot_factory)
            {
                ThiefBotFactory* regularThiefBotFactory = new ThiefBotFactory(this, x, y, ThiefBotFactory::REGULAR);
                addActor(regularThiefBotFactory);
            }
            else if (item == Level::mean_thiefbot_factory)
            {
                ThiefBotFactory* meanThiefBotFactory = new ThiefBotFactory(this, x, y, ThiefBotFactory::MEAN);
                addActor(meanThiefBotFactory);
            }
            else
                continue;
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you type q

    //setGameStatText("Game will end when you type q");
    m_avatar->doSomething();
    
    for (auto it = m_actors.begin(); it != m_actors.end(); it++) {
        if ((*it)->isAlive()) {
            (*it)->doSomething();

            if (!m_avatar->isAlive()) 
            {
                decLives();
                return GWSTATUS_PLAYER_DIED;
            }

            if (m_clear)
            {
                return GWSTATUS_FINISHED_LEVEL;
            }
        }
    }

    for (auto it = m_actors.begin(); it != m_actors.end(); it++) {
        if (!(*it)->isAlive()) {
            delete *it;
            m_actors.erase(it);
            it = m_actors.begin();
        }
    }
    
    stringstream gameDes;

    gameDes.fill('0');
    gameDes << "Score: ";
    gameDes << setw(6) << to_string(getScore()) << "  ";

    gameDes << "Level: ";
    gameDes << to_string(getLevel()) << "  ";

    gameDes << "Lives: ";
    gameDes << to_string(getLives()) << "  ";

    gameDes << "Health: ";
    gameDes << to_string(m_avatar->getHealthPercent()) << "  ";

    gameDes << "Ammo: ";
    gameDes << to_string(m_avatar->Ammo()) << "  ";

    gameDes << "Bonus: ";
    gameDes << "getBonus" << endl;

    setGameStatText(gameDes.str());
    
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

bool StudentWorld::checkMarbles(int x, int y)
{
    for (list<Actor*>::iterator actor = m_actors.begin(); actor != m_actors.end(); ++actor)
    {
        double actorX, actorY;
        actorX = (*actor)->getX();
        actorY = (*actor)->getY();

        if (actorX == x && actorY == y)
        {
            return true;
        }
    }
    return false;
}


// Can agent move to x,y?  (dx and dy indicate the direction of motion)
bool StudentWorld::canAgentMoveTo(Agent* agent, int x, int y, int dx, int dy) const
{
    int moveX = 0, moveY = 0;
    
    if (dx == -1 && dy == 0)
    {
        moveX = x - 1;
        moveY = y;
    }
    else if (dx == 1 && dy == 0)
    {
        moveX = x + 1;
        moveY = y;
    }
    else if (dx == 0 && dy == -1)
    {
        moveX = x;
        moveY = y - 1;
    }
    else if (dx == 0 && dy == 1)
    {
        moveX = x;
        moveY = y + 1;
    }
    
    for (list<Actor*>::const_iterator actor = m_actors.begin(); actor != m_actors.end(); ++actor)
    {
        if ((*actor)->getX() == moveX && (*actor)->getY() == moveY)
        {
            if ((*actor)->allowsAgentColocation()) 
            {
                return true;
            }
            else 
            {
                return false;
            }
        }
    }
    return true;
}

// Can a marble move to x,y?
bool StudentWorld::canMarbleMoveTo(int x, int y) const
{
    for (list<Actor*>::const_iterator actor = m_actors.begin(); actor != m_actors.end(); ++actor)
    {
        if ((*actor)->getX() == x && (*actor)->getY() == y)
        {
            if ((*actor)->allowsMarble() || (*actor)->allowsAgentColocation())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    return true;
}

// Is the player on the same square as an Actor?
bool StudentWorld::isPlayerColocatedWith(int x, int y) const
{
    int playerX = m_avatar->getX();
    int playerY = m_avatar->getY();

    return (playerX == x && playerY == y);
}

// Try to cause damage to something at a's location.  (a is only ever
// going to be a pea.)  Return true if something stops a -- something
// at this location prevents a pea from continuing.
bool StudentWorld::damageSomething(Actor* a, int damageAmt)
{
    int ax = a->getX();
    int ay = a->getY();
    
    for (list<Actor*>::iterator actor = m_actors.begin(); actor != m_actors.end(); ++actor)
    {
        if ((*actor)->stopsPea() && (*actor)->getX() == ax && (*actor)->getY() == ay)
        {
            (*actor)->damage(damageAmt);
            return true;
        }
    }
    return false;
}

// Swallow any swallowable object at a's location.  (a is only ever
// going to be a pit.)
bool StudentWorld::swallowSwallowable(Actor* a)
{
    int ax = a->getX();
    int ay = a->getY();
    
    for (list<Actor*>::iterator actor = m_actors.begin(); actor != m_actors.end(); ++actor) 
    {
        if ((*actor)->getX() == ax && (*actor)->getY() == ay)
        {
            (*actor)->setDead();
            a->setDead();
            return true;
        }
    }
    return false;
}

// If a pea were at x,y moving in direction dx,dy, could it hit the
// player without encountering any obstructions?
bool StudentWorld::existsClearShotToPlayer(int x, int y, int dx, int dy) const
{
    if (x == m_avatar->getX() || y == m_avatar->getY())
    {
        int currentX = x, currentY = y;
        while (currentX != m_avatar->getX() || currentY != m_avatar->getY())
        {
            if (dx == -1 && dy == 0)
            {
                currentX = x - 1;
                currentY = y;
            }
            else if (dx == 1 && dy == 0)
            {
                currentX = x + 1;
                currentY = y;
            }
            else if (dx == 0 && dy == -1)
            {
                currentX = x;
                currentY = y - 1;
            }
            else if (dx == 0 && dy == 1)
            {
                currentX = x;
                currentY = y + 1;
            }

            for (list<Actor*>::const_iterator actor = m_actors.begin(); actor != m_actors.end(); ++actor)
            {
                if ((*actor)->stopsPea())
                {
                    return false;
                }
            }
        }
        return true;
    }
    return false;
}

// If a factory is at x,y, how many items of the type that should be
// counted are in the rectangle bounded by x-distance,y-distance and
// x+distance,y+distance?  Set count to that number and return true,
// unless an item is on the factory itself, in which case return false
// and don't care about count.  (The items counted are only ever going
// ThiefBots.)
/*bool StudentWorld::doFactoryCensus(int x, int y, int distance, int& count) const
{
    count = 0;
    for (list<Actor*>::const_iterator actor = m_actors.begin(); actor != m_actors.end(); ++actor)
    {
        if ((*actor)->isShootingBot && abs((*actor)->getX() - x) <= distance && abs((*actor)->getY() - y) <= distance)
        {
            // Check if the ThiefBot is not on the factory itself
            if (!((*actor)->getX() == x && (*actor)->getY() == y))
                count++;
            else
                return false; // Item is on the factory itself
        }
    }
    return true; // Census successful
}*/

// If an item that can be stolen is at x,y, return a pointer to it;
// otherwise, return a null pointer.  (Stealable items are only ever
// going be goodies.)
Actor* StudentWorld::getColocatedStealable(int x, int y) const
{
    for (list<Actor*>::const_iterator actor = m_actors.begin(); actor != m_actors.end(); ++actor)
    {
        if ((*actor)->isStealable() && (*actor)->getX() == x && (*actor)->getY() == y)
        {
            return *actor;
        }
    }
    return nullptr;
}

// Restore player's health to the full amount.
void StudentWorld::restorePlayerHealth()
{
    m_avatar->resetHealth();
}

// Increase the amount of ammunition the player has
void StudentWorld::increaseAmmo()
{
    m_avatar->updateAmmo(20);
}

// Are there any crystals left on this level?
bool StudentWorld::anyCrystals() const
{
    for (list<Actor*>::const_iterator actor = m_actors.begin(); actor != m_actors.end(); ++actor)
    {
        if ((*actor)->isSwallowable())
        {
            return true;
        }
    }
    return false;
}

// Reduce the count of crystals on this level by 1.
bool StudentWorld::decCrystals() const
{
    for (list<Actor*>::const_iterator actor = m_actors.begin(); actor != m_actors.end(); ++actor)
    {
        if ((*actor)->isSwallowable())
        {
            // m_crystals -= 1;
            return true;
        }
    }
    return false;
}

// Indicate that the player has finished the level.
void StudentWorld::setLevelFinished()
{
    m_clear = true;
}

// Add an actor to the world
void StudentWorld::addActor(Actor* a)
{
    m_actors.push_back(a);
}

void StudentWorld::extraLife() 
{
    
}
void StudentWorld::healthReset()
{

}
void StudentWorld::morePeas() 
{

}
