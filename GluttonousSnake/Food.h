#pragma once

#include "Snake.h"

class Snake;

class Food
{
public:
	Food(const int x = 3, const int y = 3);
	~Food();
	int getX();
	int getY();
	void show(Snake &snake);
private:
	int m_nX, m_nY;
};

