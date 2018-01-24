#pragma once

#include "Comm.h"
#include "Point.h"
#include "Food.h"
#include "GameMap.h"

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
	bool checkCrashBarrier(GameMap &gMap);
	deque<Point> getSnake();
	bool eatFood(Food &food);
	void setScore(int nScore);
	void addScore(int nScore);
	int getScore();
	int getLifeVal();

	bool m_bDead;
private:
	int m_nLifeVal;
	int m_nScore;
	Direction m_eDrt;
	deque<Point> m_dqSnake;
	Point *m_ptPrev; //用于遍历蛇身时指向前一个身体 Point
};

