#include "Actor.h"
#include "StudentWorld.h"
#include "GameController.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

// ACTOR IMPLEMENTATION // 

Actor::Actor(int id, int x, int y, Direction dir, double size, unsigned int depth)
	: GraphObject(id, x, y, dir, size, depth), m_alive(true)
{
	// setVisible(true);
}

Actor::~Actor()
{
	setVisible(false);
}

bool Actor::isStillAlive()
{
	return m_alive;
}

void Actor::setDead()
{
	m_alive = false;
	setVisible(false);
}

bool Actor::doesThisBlock()
{
	return false;
}

// DIRT IMPLEMENTATION //

Dirt::Dirt(int x, int y)
	: Actor(IID_DIRT, x, y, right, 0.25, 3)
{
	setVisible(true);
}

Dirt::~Dirt()
{}

void Dirt::doSomething()
{}

// MOVEABLE OBJECTS IMPLEMENTATION

MoveableObject::MoveableObject(int id, int x, int y, Direction dir, double size, unsigned int depth, StudentWorld* world)
	: Actor(id, x, y, dir, size, depth), m_World(world)
{}

MoveableObject::~MoveableObject()
{}

StudentWorld* MoveableObject::getWorld() const
{
	return m_World;
}

// STATIONARY OBJECTS IMPLEMENTATION //

StationaryObject::StationaryObject(int id, int x, int y, Direction dir, double size, unsigned int depth, StudentWorld* world, FrackMan* fracker)
	: Actor(id, x, y, dir, size, depth), m_World(world), m_Fracker(fracker)
{}

StationaryObject::~StationaryObject()
{}

StudentWorld* StationaryObject::getWorld() const
{
	return m_World;
}

FrackMan* StationaryObject::getFracker() const
{
	return m_Fracker;
}

// FRACKMAN IMPLEMENTATION //

FrackMan::FrackMan(StudentWorld* world)
	: MoveableObject(IID_PLAYER, 30, 60, right, 1.0, 0, world), m_hp(10), m_squirts(5), m_sCharges(1), m_gold(0)
{
	setVisible(true);
}

FrackMan::~FrackMan()
{}

void FrackMan::doSomething()
{
	if (!isStillAlive())
		return;

	getWorld()->destroyDirt(getX(), getY());
	
	int dir;
	if (getWorld()->getKey(dir) == true)
	{
		switch (dir)
		{
		case KEY_PRESS_ESCAPE:
			setDead();
			break;
		case KEY_PRESS_SPACE:
			if (m_squirts > 0)
			{
				// getWorld()->insertSquirt(getX(), getY(), dir);
				m_squirts--;
			}
			break;
		case 'z':
		case 'Z':
			if (m_sCharges > 0)
			{
				getWorld()->revealCloseObjects(getX(), getY());
				GameController::getInstance().playSound(SOUND_SONAR);
				m_sCharges--;
			}
			break;
		case KEY_PRESS_TAB:
			if (m_gold > 0)
			{
				getWorld()->dropNugget(getX(), getY());
				m_gold--;
			}
			break;
		case KEY_PRESS_LEFT:
			if (getDirection() != left)
				setDirection(left);
			else if (getWorld()->canMoveHere(getX() - 1, getY()) == false)
				break;
			else if (getX() - 1 >= 0)
				moveTo(getX() - 1, getY());
			else
				moveTo(getX(), getY());
			break;
		case KEY_PRESS_RIGHT:
			if (getDirection() != right)
				setDirection(right);
			else if (getWorld()->canMoveHere(getX() + 1, getY()) == false)
				break;
			else if (getX() + 1 <= 60)
				moveTo(getX() + 1, getY());
			else
				moveTo(getX(), getY());
			break;
		case KEY_PRESS_UP:
			if (getDirection() != up)
				setDirection(up);
			else if (getWorld()->canMoveHere(getX(), getY() + 1) == false)
				break;
			else if (getY() + 1 <= 60)
				moveTo(getX(), getY() + 1);
			else
				moveTo(getX(), getY());
			break;
		case KEY_PRESS_DOWN:
			if (getDirection() != down)
				setDirection(down);
			else if (getWorld()->canMoveHere(getX(), getY() - 1) == false)
				break;
			else if (getY() - 1 >= 0)
				moveTo(getX(), getY() - 1);
			else
				moveTo(getX(), getY());
			break;
		}
	}
}

int FrackMan::getHP() const
{
	return m_hp;
}

int FrackMan::getSquirts() const
{
	return m_squirts;
}

int FrackMan::getsCharges() const
{
	return m_sCharges;
}

int FrackMan::getGold() const
{
	return m_gold;
}

void FrackMan::addHP() 
{
	m_hp++;
}

void FrackMan::addSquirts() 
{
	m_squirts += 5;
}

void FrackMan::addCharges() 
{
	m_sCharges++;
}

void FrackMan::addGold() 
{
	m_gold++;
}

// BOULDER IMPLEMENTATION //

Boulder::Boulder(int x, int y, StudentWorld* world)
	: MoveableObject(IID_BOULDER, x, y, down, 1.0, 1, world), m_state(0), m_tickLife(30), m_x(getX()), m_y(getY())
{
	setVisible(true);
	getWorld()->destroyDirt(x, y);
}

Boulder::~Boulder()
{}

bool Boulder::isAnyDirtUnderBoulder()
{
	if (getWorld()->isThereDirtHere(getX(), getY() - 1) || getWorld()->isThereDirtHere(getX() + 1, getY() - 1) ||
		getWorld()->isThereDirtHere(getX() + 2, getY() - 1) || getWorld()->isThereDirtHere(getX() + 3, getY() - 1))
	{
		return true;
	}
	return false;
}

void Boulder::doSomething()
{
	if (!isStillAlive())
		return;

	// if Boulder is in a stable state
	if (m_state == 0)
	{
		if (!isAnyDirtUnderBoulder())
		{
			m_state = 1;
		}
	}
	// if Boulder is in a waiting state
	else if (m_state == 1)
	{
		if (m_tickLife == 0)
		{
			m_state = -1;
			// GameController::getInstance().playSound(SOUND_FALLING_ROCK);
		}
		m_tickLife--;
	}
	// if Boulder is in a falling state
	else 
	{
		if (getY() > 0 && !isAnyDirtUnderBoulder())
		{
			moveTo(getX(), getY() - 1);
			if (getWorld()->isCollidingWith(getX(), getY(), getWorld()->getFrack()))
			{
				getWorld()->getFrack()->setDead();
				setDead();
			}
		}
		else
			setDead();
	}

	return;
}

int Boulder::getBoulderX()
{
	return m_x;
}

int Boulder::getBoulderY()
{
	return m_y;
}

bool Boulder::doesThisBlock()
{
	return true;
}

// SQUIRT IMPLEMENTATION //

Squirt::Squirt(int x, int y, Direction dir, StudentWorld* world, FrackMan* fracker)
	: StationaryObject(IID_WATER_SPURT, x, y, dir, 1.0, 1, world, fracker), m_distanceTrav(0)
{
	setVisible(true);
}

Squirt::~Squirt()
{}

void Squirt::doSomething()
{
	/*
	If a Squirt is within a radius of 3.0 of one or more Protesters (up to and including
	a distance of 3.0 squares away), it will cause 2 points of annoyance to these
	Protester(s), and then immediately set its state to dead, so it can be removed from
	the oil field at the end of the tick
	*/

	if (isStillAlive() && m_distanceTrav <= 4)
	{
		m_distanceTrav++;
	}
	else
	{
		setDead();
	}
}

// Barrel IMPLEMENTATION //

Barrel::Barrel(int x, int y, StudentWorld* world, FrackMan* fracker)
	: StationaryObject(IID_BARREL, x, y, right, 1.0, 2, world, fracker) 
{
	// setVisible(true); // for testing only, delete after
}

Barrel::~Barrel()
{}

void Barrel::doSomething()
{
	if (!isStillAlive())
		return;

	if (isVisible() == false && getWorld()->isWithinRadius(getX(), getY(), getFracker()->getX(), getFracker()->getY(), 4))
	{
		setVisible(true);
		return;
	}
	else if (getWorld()->isCollidingWith(getX(), getY(), getFracker()))
	{
		setDead();
		GameController::getInstance().playSound(SOUND_FOUND_OIL);
		getWorld()->increaseScore(1000);
		getWorld()->reduceBarrels();
	}
}

// GOLD NUGGET IMPLEMENTATION //

GoldNugget::GoldNugget(int x, int y, bool isPerm, StudentWorld* world, FrackMan* fracker)
	: StationaryObject(IID_GOLD, x, y, right, 1.0, 2, world, fracker), isPermanentState(isPerm), m_tickLife(300)
{
	if (isPermanentState)
	{
		setVisible(true);
		canFrackManGet = true;
	}
	else
	{
		setVisible(true);
		canFrackManGet = false;
	}
}

GoldNugget::~GoldNugget()
{}

void GoldNugget::doSomething()
{
	if (!isStillAlive())
		return;

	if (!isVisible() && getWorld()->isWithinRadius(getX(), getY(), getFracker()->getX(), getFracker()->getY(), 4.0))
	{
		setVisible(true);
		return;
	}
	else if (canFrackManGet == true && getWorld()->isCollidingWith(getX(), getY(), getFracker()))
	{
		setDead();
		GameController::getInstance().playSound(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(10);
		getFracker()->addGold();
	}
	else if (canFrackManGet == false) // and if it is within 3 units of a Protester
	{
		setDead();
		GameController::getInstance().playSound(SOUND_PROTESTER_FOUND_GOLD);
		// The Gold Nugget must tell the Protester object to be bribed and act accordingly
		getWorld()->increaseScore(25);
	}

	if (!isPermanentState)
	{
		if (m_tickLife == 0)
		{
			setDead();
			return;
		}
		m_tickLife--;
	}
}

// SONAR KIT IMPLEMENTATION //

SonarKit::SonarKit(int x, int y, StudentWorld* world, FrackMan* fracker)
	: StationaryObject(IID_SONAR, x, y, right, 1.0, 2, world, fracker)
{
	m_tickLife = getWorld()->max(100, 300 - 10 * getWorld()->getLevel());
	setVisible(true);
}

SonarKit::~SonarKit()
{}

void SonarKit::doSomething()
{
	if (!isStillAlive())
		return;

	if (getWorld()->isCollidingWith(getX(), getY(), getFracker()))
	{
		setDead();
		GameController::getInstance().playSound(SOUND_GOT_GOODIE);
		getFracker()->addCharges();
		getWorld()->increaseScore(75);
	}

	if (m_tickLife == 0)
	{
		setDead();
		return;
	}
	m_tickLife--;
}

// WATER POOL IMPLEMENTATION //

WaterPool::WaterPool(int x, int y, StudentWorld* world, FrackMan* fracker)
	: StationaryObject(IID_WATER_POOL, x, y, right, 1.0, 2, world, fracker)
{
	m_tickLife = getWorld()->max(100, 300 - 10 * getWorld()->getLevel());
	setVisible(true);
}

WaterPool::~WaterPool()
{}

void WaterPool::doSomething()
{
	if (!isStillAlive())
		return;

	if (getWorld()->isWithinRadius(getX(), getY(), getFracker()->getX(), getFracker()->getY(), 3.0))
	{
		setDead();
		GameController::getInstance().playSound(SOUND_GOT_GOODIE);
		getFracker()->addSquirts();
		getWorld()->increaseScore(100);
	}

	if (m_tickLife == 0)
	{
		setDead();
		return;
	}
	m_tickLife--;
}