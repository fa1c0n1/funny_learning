#pragma once

#include "Snake.h"
#include "GameMap.h"

class Snake;

class Food
{
public:
	Food(const int x = 0, const int y = 0);
	~Food();
	int getX();
	int getY();
	void show(Snake &snake, GameMap &gMap);
private:
	void show();

	int m_nX, m_nY;
};

