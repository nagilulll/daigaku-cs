#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(StudentWorld* world, int startX, int startY, int imageID, int hitPoints, int startDir) : GraphObject(imageID, startX, startY, startDir), m_world(world), m_hitPoints(hitPoints), m_alive(true)
{
}

bool Actor::isAlive() const
{
    return m_alive;
}

void Actor::setDead()
{
    m_alive = false;
}

void Actor::decHitPoints(int amt)
{
    m_hitPoints -= amt;
    if (m_hitPoints <= 0) 
    {
        setDead();
    }
}

StudentWorld* Actor::getWorld() const
{
    return m_world;
}

  // Can an agent occupy the same square as this actor?
bool Actor::allowsAgentColocation() const
{
    return false;
}

  // Can a marble occupy the same square as this actor?
bool Actor::allowsMarble() const
{
    return false;
}

  // Does this actor count when a factory counts items near it?
bool Actor::countsInFactoryCensus() const
{
    return false;
}

  // Does this actor stop peas from continuing?
bool Actor::stopsPea() const
{
    return false;
}

  // Can this actor be damaged by peas?
bool Actor::isDamageable() const
{
    return true;
}

  // Cause this Actor to sustain damageAmt hit points of damage.
void Actor::damage(int damageAmt)
{
    if (isDamageable()) 
    {
        decHitPoints(damageAmt);
    }
}

  // Can this actor be pushed by a to location x,y?
bool Actor::bePushedBy(Agent* a, int x, int y)
{
    return false;
}

  // Can this actor be swallowed by a pit?
bool Actor::isSwallowable() const
{
    return false;
}

  // Can this actor be picked up by a ThiefBot?
bool Actor::isStealable() const
{
    return false;
}

  // How many hit points does this actor have left?
int Actor::getHitPoints() const
{
    return m_hitPoints;
}

  // Set this actor's hit points to amt.
void Actor::setHitPoints(int amt)
{
    m_hitPoints = amt;
}

  // Make the actor sustain damage.  Return true if this kills the
  // actor, and false otherwise.
bool Actor::tryToBeKilled(int damageAmt)
{
    damage(damageAmt);
    return !isAlive();
}

// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

Agent::Agent(StudentWorld* world, int startX, int startY, int imageID, int hitPoints, int startDir) : Actor(world, startX, startY, imageID, hitPoints, startDir)
{

}

// Move to the adjacent square in the direction the agent is facing
// if it is not blocked, and return true.  Return false if the agent
// can't move.
bool Agent::moveIfPossible()
{
    return true;
}

// Return true if this agent can push marbles (which means it's the
// player).
bool Agent::canPushMarbles() const
{
    return false;
}

// Return true if this agent doesn't shoot unless there's an unobstructed
// path to the player.
bool Agent::needsClearShot() const
{
    return false;
}

// Return the sound effect ID for a shot from this agent.
int Agent::shootingSound() const
{
    return SOUND_PLAYER_FIRE;
}


// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

Avatar::Avatar(StudentWorld* world, int startX, int startY) : Agent(world, startX, startY, IID_PLAYER, 20, right)
{
    updateAmmo(20);
    setVisible(true);
}

int Avatar::Ammo() const
{
    return m_peas;
}

void Avatar::resetHealth()
{
    setHitPoints(20);
}

int Avatar::getHealthPercent() const
{
    int hitPoints = getHitPoints();
    if (hitPoints <= 0) {
        return 0;
    }
    return (hitPoints / 20) * 100;
}

bool Avatar::isDamageable() const
{
    return true;
}

void Avatar::damage(int damageAmt)
{
    decHitPoints(damageAmt);
    if (getHitPoints() <= 0) {
        setDead();
    }
}

bool Avatar::canPushMarbles() const
{
    return true;
}

bool Avatar::needsClearShot() const
{
    return false;
}

int Avatar::shootingSound() const
{
    return SOUND_PLAYER_FIRE;
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
            case KEY_PRESS_SPACE:
                shoot();
                break;
            /*case KEY_PRESS_ESCAPE:
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
            --currentX;
            break;
        case right:
            ++currentX;
            break;
        case up:
            ++currentY;
            break;
        case down:
            --currentY;
            break;
    }
    int marbleNewX = currentX, marbleNewY = currentY;

            switch (dir)
            {
                case left:
                    --marbleNewX;
                    break;
                case right:
                    ++marbleNewX;
                    break;
                case up:
                    ++marbleNewY;
                    break;
                case down:
                    --marbleNewY;
                    break;
            }
    
        Actor* actors = nullptr;
        if (getWorld()->occupy(currentX, currentY, actors))
        {
            if (actors->isSwallowable() && getWorld()->canMarbleMoveTo(marbleNewX, marbleNewY))
            {
                Actor* newActor = nullptr;
                if (getWorld()->occupy(marbleNewX, marbleNewY, newActor))
                {
                    actors->moveTo(marbleNewX, marbleNewY);
                    if (newActor->allowsMarble())
                        getWorld()->swallowSwallowable(newActor);
                }
                actors->moveTo(marbleNewX, marbleNewY);
                setDirection(dir);
                moveTo(currentX, currentY);
            }
            else if (actors->allowsAgentColocation())
            {
                actors->setDead();
                setDirection(dir);
                moveTo(currentX, currentY);
            }
            else
            {
            }
        }
        else if (!getWorld()->occupy(currentX, currentY, actors))
        {
            setDirection(dir);
            moveTo(currentX, currentY);
        }
}

void Avatar::shoot()
{
    if (m_peas > 0) {
        int dir = getDirection();
        double x = getX(), y = getY();
        getWorld()->addActor(new Pea(getWorld(), x, y, dir));
        getWorld()->playSound(SOUND_PLAYER_FIRE);
        m_peas--;
    }
}

void Avatar::updateAmmo(int peas)
{
    m_peas += peas;
}

void goodieLife(int life) 
{

}
void goodieHealth(int health) 
{

}
void goodiePeas(int peas) 
{

}



// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||


Robot::Robot(StudentWorld* world, int startX, int startY, int imageID, int hitPoints, int score, int startDir) : Agent(world, startX, startY, imageID, hitPoints, startDir), m_score(score)
{
}

void Robot::doSomething() const
{
    return;
}
bool Robot::isDamageable() const
{
    return true;
}
void Robot::damage(int damageAmt)
{
    decHitPoints(damageAmt);
    if (getHitPoints() <= 0) {
        setDead();
    }
}
bool Robot::canPushMarbles() const
{
    return false;
}
bool Robot::needsClearShot() const
{
    return true;
}
int Robot::shootingSound() const
{
    return SOUND_ENEMY_FIRE;
}

  // Does this robot shoot?
bool Robot::isShootingRobot() const
{
    return false;
}


// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||


RageBot::RageBot(StudentWorld* world, int startX, int startY, int startDir) : Robot(world, startX, startY, IID_RAGEBOT, 10, 100, startDir)
{
    setVisible(true);
}

void RageBot::doSomething()
{
    if (!isAlive())
        return;

    int currentX = getX();
    int currentY = getY();

    int nextX = currentX;
    int nextY = currentY;
    switch (getDirection()) {
        case up:
            nextY++;
            break;
        case down:
            nextY--;
            break;
        case left:
            nextX--;
            break;
        case right:
            nextX++;
            break;
    }

    Actor* actor = nullptr;
    if (getWorld()->occupy(nextX, nextY, actor)) 
    {
        setDirection(reverseDirection(getDirection()));
    }
    else
    {
        moveTo(nextX, nextY);
    }
}

int RageBot::reverseDirection(int dir) const
{
    switch (dir) 
    {
        case up:
            return down;
        case down:
            return up;
        case left:
            return right;
        case right:
            return left;
        default:
            return none;
    }
}


// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||


ThiefBot::ThiefBot(StudentWorld* world, int startX, int startY, int imageID, int hitPoints, int score) : Robot(world, startX, startY, imageID, hitPoints, score, right)
{
}
void ThiefBot::doSomething()
{

}
bool ThiefBot::countsInFactoryCensus() const
{
    return true;
}
void ThiefBot::damage(int damageAmt)
{
    Robot::damage(damageAmt);
}


// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||


RegularThiefBot::RegularThiefBot(StudentWorld* world, int startX, int startY) : ThiefBot(world, startX, startY, IID_THIEFBOT, 5, 10)
{
    
}
void RegularThiefBot::doSomething()
{
    if (!isAlive())
        return;

    int currentX = getX();
    int currentY = getY();

    int nextX = currentX;
    int nextY = currentY;
    switch (getDirection()) {
        case up:
            nextY++;
            break;
        case down:
            nextY--;
            break;
        case left:
            nextX--;
            break;
        case right:
            nextX++;
            break;
    }

    Actor* actor = nullptr;
    if (getWorld()->occupy(nextX, nextY, actor))
    {
    }
    else
    {
        moveTo(nextX, nextY);
    }
}
bool RegularThiefBot::isShootingRobot() const
{
    return false;
}

// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||


MeanThiefBot::MeanThiefBot(StudentWorld* world, int startX, int startY) : ThiefBot(world, startX, startY, IID_MEAN_THIEFBOT, 8, 20)
{
    
}
void MeanThiefBot::doSomething()
{
    if (!isAlive())
        return;

    int currentX = getX();
    int currentY = getY();

    int nextX = currentX;
    int nextY = currentY;
    switch (getDirection()) {
        case up:
            nextY++;
            break;
        case down:
            nextY--;
            break;
        case left:
            nextX--;
            break;
        case right:
            nextX++;
            break;
    }

    Actor* actor = nullptr;
    if (getWorld()->occupy(nextX, nextY, actor))
    {
    }
    else
    {
        moveTo(nextX, nextY);
    }
}

// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||


Exit::Exit(StudentWorld* world, int startX, int startY) : Actor(world, startX, startY, IID_EXIT, 1, right)
{
    setVisible(false);
}

void Exit::doSomething()
{
    if (!getWorld()->anyCrystals())
        setVisible(true);
    if (getWorld()->isPlayerColocatedWith(getX(), getY()) && isVisible())
    {
        getWorld()->playSound(SOUND_FINISHED_LEVEL);
        getWorld()->increaseScore(2000);
        getWorld()->setLevelFinished();
    }
}

bool Exit::allowsAgentColocation() const
{
    return true;
}

// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||


Marble::Marble(StudentWorld* world, int startX, int startY) : Actor(world, startX, startY, IID_MARBLE, 10, none)
{
    setVisible(true);
}
void Marble::doSomething()
{
    
}
bool Marble::isDamageable() const
{
    return true;
}
void Marble::damage(int damageAmt)
{
    
}
bool Marble::isSwallowable() const
{
    return true;
}
bool Marble::bePushedBy(Agent* a, int x, int y)
{

    if (getWorld()->canMarbleMoveTo(x, y) && a->canPushMarbles())
    {
        moveTo(x, y);
        return true;
    }
    return false;
}


// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||


Pit::Pit(StudentWorld* world, int startX, int startY) : Actor(world, startX, startY, IID_PIT, 1, none)
{
    setVisible(true);
}
void Pit::doSomething()
{
/*
    if (!isAlive())
        return;
    
    int currentX = getX();
    int currentY = getY();

    Marble* marble = nullptr;
    Pit* pit = nullptr;
    int marbleX = marble->getX();
    int marbleY = marble->getY();
    
    if (currentX == marbleX && currentY == marbleY)
    {
        getWorld()->swallowSwallowable(pit);
    }
 */
}
bool Pit::allowsMarble() const
{
    return true;
}

// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||


Pea::Pea(StudentWorld* world, int startX, int startY, int startDir) : Actor(world, startX, startY, IID_PEA, 1, startDir)
{
    setVisible(true);
}

void Pea::doSomething()
{
    if (!isAlive())
        return;
    
    int targetX = getX();
    int targetY = getY();
    if (getDirection() == left) targetX--;
    else if (getDirection() == right) targetX++;
    else if (getDirection() == up) targetY++;
    else if (getDirection() == down) targetY--;

    
    // Check if there's an object at the target position
    Actor* target = nullptr;
    if (!getWorld()->occupy(targetX, targetY, target))
    {
        if (target != nullptr && target->allowsAgentColocation()) {
            // Handle interaction with the target object
            if (target->allowsAgentColocation()) {
                target->damage(2); // Damage the object
                setDead(); // Set the pea's state to dead
                return;
            }
            else if (target->stopsPea()) 
            {
                setDead();
                return;
            }
        }
    }

    // Move the pea to the target position
    moveTo(targetX, targetY);

    // Check again for collision at the new position
    if (!getWorld()->occupy(targetX, targetY, target))
    {
        if (target != nullptr && target->allowsAgentColocation()) 
        {
            target->damage(2); // Damage the object
            setDead(); // Set the pea's state to dead
        }
        else if (target->stopsPea())
        {
            setDead();
            return;
        }
    }

}

bool Pea::allowsAgentColocation() const
{
    return true;
}


// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

ThiefBotFactory::ThiefBotFactory(StudentWorld* world, int startX, int startY, ProductType type) : Actor(world, startX, startY, IID_ROBOT_FACTORY, 1, right), m_productType(type)
{
    
}
void ThiefBotFactory::doSomething()
{
    
}
bool ThiefBotFactory::stopsPea() const
{
    return true; 
}

// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||


PickupableItem::PickupableItem(StudentWorld* world, int startX, int startY, int imageID, int score) : Actor(world, startX, startY, imageID, 1, none), m_score(score)
{
    
}
void PickupableItem::doSomething()
{
    
}
bool PickupableItem::allowsAgentColocation() const
{
    return true;
}

// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||


Crystal::Crystal(StudentWorld* world, int startX, int startY) : PickupableItem(world, startX, startY, IID_CRYSTAL, 50)
{
}

// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||


Goodie::Goodie(StudentWorld* world, int startX, int startY, int imageID, int score) : PickupableItem(world, startX, startY, imageID, score), m_stolen(false)
{
    
}
void Goodie::doSomething()
{
    if (!isAlive())
        return;
    if (getWorld()->isPlayerColocatedWith(getX(), getY())) 
    {
        getWorld()->playSound(SOUND_GOT_GOODIE);
        goodieBuff();
        setDead();
        return;
    }
}
bool Goodie::isStealable() const
{
    return true;
}
  // Set whether this goodie is currently stolen.
void Goodie::setStolen(bool status)
{
    m_stolen = status;
}

// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

ExtraLifeGoodie::ExtraLifeGoodie(StudentWorld* world, int startX, int startY) : Goodie(world, startX, startY, IID_EXTRA_LIFE, 1000)
{
}

void ExtraLifeGoodie::goodieBuff()
{
    getWorld()->increaseScore(1000);
    getWorld()->extraLife();
}


RestoreHealthGoodie::RestoreHealthGoodie(StudentWorld* world, int startX, int startY) : Goodie(world, startX, startY, IID_RESTORE_HEALTH, 500)
{
}

void RestoreHealthGoodie::goodieBuff()
{
    getWorld()->increaseScore(500);
    getWorld()->healthReset();
}

AmmoGoodie::AmmoGoodie(StudentWorld* world, int startX, int startY) : Goodie(world, startX, startY, IID_AMMO, 100)
{
}

void AmmoGoodie::goodieBuff()
{
    getWorld()->increaseScore(100);
    getWorld()->morePeas();
}

// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||


Wall::Wall(StudentWorld* world, int startX, int startY) : Actor(world, startX, startY, IID_WALL, 0, none)
{
    setVisible(true);
}

void Wall::doSomething()
{
}

bool Wall::stopsPea() const
{
    return true;
}
