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
	Snake(bool bInitBody = true);
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
	void setLifeVal(int nLifeVal);
	int getLifeVal();
	int getSnakeLen();
	void addBody(Point &point);

	bool m_bDead;
private:
	int m_nLifeVal;
	int m_nScore;
	int m_nSnakeLen;
	bool m_bInitBody;
	Direction m_eDrt;
	deque<Point> m_dqSnake;
	Point *m_ptPrev; //用于遍历蛇身时指向前一个身体 Point
};

