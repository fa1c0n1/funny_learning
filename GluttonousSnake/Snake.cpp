#include "Snake.h"
#include "Point.h"
#include "DrawTool.h"

Snake::Snake()
	: m_nLifeVal(SNAKE_INIT_LIFE), m_eDrt(DRT_UP), m_bDead(false), 
	m_ptPrev(nullptr), m_nScore(0)
{
	this->m_dqSnake.emplace_back(Point(25, 28));
	this->m_dqSnake.emplace_back(Point(25, 29));
	this->m_dqSnake.emplace_back(Point(25, 30));
}

Snake::~Snake()
{
	if (this->m_bDead) {
		if (!this->m_dqSnake.empty())
			this->m_dqSnake.clear();
	}

	if (this->m_ptPrev != nullptr) {
		delete this->m_ptPrev;
		this->m_ptPrev = nullptr;
	}
}
	
void Snake::show()
{
	DrawTool::SetColor(FG_LIGHTYELLOW);
	for (auto &point : this->m_dqSnake) {
		point.printCircle();
	}
}

void Snake::move()
{
	for (auto &point : this->m_dqSnake)
	{
		point.clear();
	
		if (this->m_ptPrev != nullptr) { //����
			int tx = point.getX();
			int ty = point.getY();
			point.changePosition(this->m_ptPrev->getX(), this->m_ptPrev->getY());
			this->m_ptPrev->setX(tx);
			this->m_ptPrev->setY(ty);
		}
		else { // ��ͷ
			this->m_ptPrev = new Point(0, 0);
			this->m_ptPrev->setX(point.getX());
			this->m_ptPrev->setY(point.getY());
			switch (this->m_eDrt)
			{
			case DRT_UP:
				point.changePosition(point.getX(), point.getY() - 1);
				break;
			case DRT_DOWN:
				point.changePosition(point.getX(), point.getY() + 1);
				break;
			case DRT_LEFT:
				point.changePosition(point.getX() - 1, point.getY());
				break;
			case DRT_RIGHT:
				point.changePosition(point.getX() + 1, point.getY());
				break;
			}
		}

		DrawTool::SetColor(FG_LIGHTYELLOW);
		point.printCircle();
	}

	if (this->m_ptPrev != nullptr) {
		delete this->m_ptPrev;
		this->m_ptPrev = nullptr;
	}
}

void Snake::setDirection(Direction eDrt)
{
	this->m_eDrt = eDrt;
}


Direction Snake::getDirection()
{
	return this->m_eDrt;
}

deque<Point> Snake::getSnake()
{
	return this->m_dqSnake;
}

bool Snake::checkCrashWall()
{
	Point snakeHead = this->m_dqSnake.front();
	int nX = snakeHead.getX();
	int nY = snakeHead.getY();

	if (nX <= 1 || nX >= 32 || nY <= 1 || nY >= 32) {
		this->m_nLifeVal = 0;
		return true;
	}


	return false;
}

bool Snake::checkCrashBarrier(GameMap &gMap)
{
	vector<Point> vtBarriers = gMap.getBarrier()->getBarriers();
	Point snakeHead = this->m_dqSnake.front();
	int nX = snakeHead.getX();
	int nY = snakeHead.getY();

	for (auto &point : vtBarriers) {
		if (nX == point.getX() && nY == point.getY()) {
			this->m_nLifeVal -= 1;
			return true;
		}
	}

	return false;
}

bool Snake::eatFood(Food &food)
{
	Point snakeHead = this->m_dqSnake.front();
	Point snakeTail = this->m_dqSnake.back();
	if (snakeHead.getX() == food.getX() && snakeHead.getY() == food.getY()) {
		this->m_dqSnake.emplace_back(Point(snakeTail));
		this->m_nScore += 10;
		return true;
	}

	return false;
}

void Snake::setScore(int nScore)
{
	this->m_nScore = nScore;
}

void Snake::addScore(int nScore)
{
	this->m_nScore += nScore;
}

int Snake::getScore()
{
	return this->m_nScore;
}

int Snake::getLifeVal()
{
	return this->m_nLifeVal;
}
