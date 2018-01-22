#include "Food.h"
#include "Snake.h"
#include "DrawTool.h"

#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

Food::Food(const int x, const int y)
	: m_nX(y), m_nY(x)
{
}


Food::~Food()
{
}
	
void Food::show(Snake &snake)
{
	deque<Point> dqSnake = snake.getSnake();
	bool bConcident = false;
	while (true) {
		srand((unsigned int)time(NULL));
		this->m_nX = rand() % 30 + 2;
		this->m_nY = rand() % 30 + 2;

		for (auto &point : dqSnake) {
			if (this->m_nX == point.getX() && this->m_nY == point.getY()) {
				bConcident = true;
				break;
			}
		}

		if (bConcident)
			continue;
		else 
			break;
	}

	DrawTool::SetColor(FG_LIGHTPURPLE);
	DrawTool::SetCursorPosition(this->m_nX, this->m_nY);
	cout << "бя";
}


int Food::getX()
{
	return this->m_nX;
}

int Food::getY()
{
	return this->m_nY;
}

