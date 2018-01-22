#pragma once

#include "Comm.h"
#include "Point.h"
#include "Food.h"

#include <deque>

using namespace std;

class Food;

class Snake
{
public:
	Snake();
	~Snake();
	void setDirection(Direction eDrt);
	Direction getDirection();
	void move();
	void show();
	bool checkCrashWall();
	deque<Point> getSnake();
	bool eatFood(Food &food);

	bool m_bDead;
private:
	int m_nLifeVal;
	int m_nScore;
	Direction m_eDrt;
	deque<Point> m_dqSnake;
	Point *m_ptPrev;
};

