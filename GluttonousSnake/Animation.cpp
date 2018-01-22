#include "Animation.h"

#include <windows.h>

Animation::Animation()
	: m_nSpeed(20)
{
	// Initalize the points of start snake.
	this->dqStartSnake.emplace_back(Point(0, 15));
	this->dqStartSnake.emplace_back(Point(1, 14));
	this->dqStartSnake.emplace_back(Point(2, 13));
	this->dqStartSnake.emplace_back(Point(3, 12));
	this->dqStartSnake.emplace_back(Point(4, 13));
	this->dqStartSnake.emplace_back(Point(5, 14));
	this->dqStartSnake.emplace_back(Point(6, 15));
	this->dqStartSnake.emplace_back(Point(7, 14));
	this->dqStartSnake.emplace_back(Point(8, 13));
	this->dqStartSnake.emplace_back(Point(9, 12));

	// Initialize the points of start text: GLUTTONOUS
	int dx = -43;
	this->vtStartText1.emplace_back(Point(0+dx, 11)); //G
	this->vtStartText1.emplace_back(Point(0+dx, 12));
	this->vtStartText1.emplace_back(Point(0+dx, 13));
	this->vtStartText1.emplace_back(Point(1+dx, 10));
	this->vtStartText1.emplace_back(Point(1+dx, 13));
	this->vtStartText1.emplace_back(Point(2+dx, 10));
	this->vtStartText1.emplace_back(Point(2+dx, 12));
	this->vtStartText1.emplace_back(Point(2+dx, 13));
	this->vtStartText1.emplace_back(Point(2+dx, 14));

	this->vtStartText1.emplace_back(Point(4+dx, 10)); //L
	this->vtStartText1.emplace_back(Point(4+dx, 11));
	this->vtStartText1.emplace_back(Point(4+dx, 12));
	this->vtStartText1.emplace_back(Point(4+dx, 13));
	this->vtStartText1.emplace_back(Point(4+dx, 14));
	this->vtStartText1.emplace_back(Point(5+dx, 14));
	this->vtStartText1.emplace_back(Point(6+dx, 14));

	this->vtStartText1.emplace_back(Point(8+dx, 10)); //U
	this->vtStartText1.emplace_back(Point(8+dx, 11)); 
	this->vtStartText1.emplace_back(Point(8+dx, 12)); 
	this->vtStartText1.emplace_back(Point(8+dx, 13)); 
	this->vtStartText1.emplace_back(Point(9+dx, 14)); 
	this->vtStartText1.emplace_back(Point(10+dx, 14)); 
	this->vtStartText1.emplace_back(Point(11+dx, 10)); 
	this->vtStartText1.emplace_back(Point(11+dx, 11)); 
	this->vtStartText1.emplace_back(Point(11+dx, 12)); 
	this->vtStartText1.emplace_back(Point(11+dx, 13)); 

	this->vtStartText1.emplace_back(Point(13+dx, 10)); //T
	this->vtStartText1.emplace_back(Point(14+dx, 10));
	this->vtStartText1.emplace_back(Point(14+dx, 11));
	this->vtStartText1.emplace_back(Point(14+dx, 12));
	this->vtStartText1.emplace_back(Point(14+dx, 13));
	this->vtStartText1.emplace_back(Point(14+dx, 14));
	this->vtStartText1.emplace_back(Point(15+dx, 10));

	this->vtStartText1.emplace_back(Point(17+dx, 10)); //T
	this->vtStartText1.emplace_back(Point(18+dx, 10));
	this->vtStartText1.emplace_back(Point(18+dx, 11));
	this->vtStartText1.emplace_back(Point(18+dx, 12));
	this->vtStartText1.emplace_back(Point(18+dx, 13));
	this->vtStartText1.emplace_back(Point(18+dx, 14));
	this->vtStartText1.emplace_back(Point(19+dx, 10));

	this->vtStartText1.emplace_back(Point(21+dx, 11)); //O
	this->vtStartText1.emplace_back(Point(21+dx, 12));
	this->vtStartText1.emplace_back(Point(21+dx, 13));
	this->vtStartText1.emplace_back(Point(22+dx, 10));
	this->vtStartText1.emplace_back(Point(22+dx, 14));
	this->vtStartText1.emplace_back(Point(23+dx, 11));
	this->vtStartText1.emplace_back(Point(23+dx, 12));
	this->vtStartText1.emplace_back(Point(23+dx, 13));

	this->vtStartText1.emplace_back(Point(25+dx, 10)); //N
	this->vtStartText1.emplace_back(Point(25+dx, 11));
	this->vtStartText1.emplace_back(Point(25+dx, 12));
	this->vtStartText1.emplace_back(Point(25+dx, 13));
	this->vtStartText1.emplace_back(Point(25+dx, 14));
	this->vtStartText1.emplace_back(Point(26+dx, 11));
	this->vtStartText1.emplace_back(Point(27+dx, 12));
	this->vtStartText1.emplace_back(Point(28+dx, 13));
	this->vtStartText1.emplace_back(Point(29+dx, 10));
	this->vtStartText1.emplace_back(Point(29+dx, 11));
	this->vtStartText1.emplace_back(Point(29+dx, 12));
	this->vtStartText1.emplace_back(Point(29+dx, 13));
	this->vtStartText1.emplace_back(Point(29+dx, 14));

	this->vtStartText1.emplace_back(Point(31+dx, 11)); //O
	this->vtStartText1.emplace_back(Point(31+dx, 12));
	this->vtStartText1.emplace_back(Point(31+dx, 13));
	this->vtStartText1.emplace_back(Point(32+dx, 10));
	this->vtStartText1.emplace_back(Point(32+dx, 14));
	this->vtStartText1.emplace_back(Point(33+dx, 11));
	this->vtStartText1.emplace_back(Point(33+dx, 12));
	this->vtStartText1.emplace_back(Point(33+dx, 13));
	
	this->vtStartText1.emplace_back(Point(35+dx, 10)); //U
	this->vtStartText1.emplace_back(Point(35+dx, 11));
	this->vtStartText1.emplace_back(Point(35+dx, 12));
	this->vtStartText1.emplace_back(Point(35+dx, 13));
	this->vtStartText1.emplace_back(Point(36+dx, 14));
	this->vtStartText1.emplace_back(Point(37+dx, 14));
	this->vtStartText1.emplace_back(Point(38+dx, 10));
	this->vtStartText1.emplace_back(Point(38+dx, 11));
	this->vtStartText1.emplace_back(Point(38+dx, 12));
	this->vtStartText1.emplace_back(Point(38+dx, 13));

	this->vtStartText1.emplace_back(Point(40+dx, 11)); //S
	this->vtStartText1.emplace_back(Point(40+dx, 14)); 
	this->vtStartText1.emplace_back(Point(41+dx, 10)); 
	this->vtStartText1.emplace_back(Point(41+dx, 12)); 
	this->vtStartText1.emplace_back(Point(41+dx, 14)); 
	this->vtStartText1.emplace_back(Point(42+dx, 10)); 
	this->vtStartText1.emplace_back(Point(42+dx, 13)); 

	// Initialize the points of start text: SNAKE.
	dx = -28;
	this->vtStartText2.emplace_back(Point(0+dx, 21)); //S
	this->vtStartText2.emplace_back(Point(0+dx, 24));
	this->vtStartText2.emplace_back(Point(1+dx, 20));
	this->vtStartText2.emplace_back(Point(1+dx, 22));
	this->vtStartText2.emplace_back(Point(1+dx, 24));
	this->vtStartText2.emplace_back(Point(2+dx, 20));
	this->vtStartText2.emplace_back(Point(2+dx, 23));

	this->vtStartText2.emplace_back(Point(4+dx, 20)); //N
	this->vtStartText2.emplace_back(Point(4+dx, 21));
	this->vtStartText2.emplace_back(Point(4+dx, 22)); 
	this->vtStartText2.emplace_back(Point(4+dx, 23)); 
	this->vtStartText2.emplace_back(Point(4+dx, 24)); 
	this->vtStartText2.emplace_back(Point(5+dx, 21)); 
	this->vtStartText2.emplace_back(Point(6+dx, 22)); 
	this->vtStartText2.emplace_back(Point(7+dx, 23)); 
	this->vtStartText2.emplace_back(Point(8+dx, 20));
	this->vtStartText2.emplace_back(Point(8+dx, 21));
	this->vtStartText2.emplace_back(Point(8+dx, 22));
	this->vtStartText2.emplace_back(Point(8+dx, 23));
	this->vtStartText2.emplace_back(Point(8+dx, 24));

	this->vtStartText2.emplace_back(Point(10+dx, 24)); //A
	this->vtStartText2.emplace_back(Point(11+dx, 23)); 
	this->vtStartText2.emplace_back(Point(12+dx, 22)); 
	this->vtStartText2.emplace_back(Point(13+dx, 21)); 
	this->vtStartText2.emplace_back(Point(13+dx, 22)); 
	this->vtStartText2.emplace_back(Point(14+dx, 20)); 
	this->vtStartText2.emplace_back(Point(14+dx, 22)); 
	this->vtStartText2.emplace_back(Point(15+dx, 21)); 
	this->vtStartText2.emplace_back(Point(15+dx, 22)); 
	this->vtStartText2.emplace_back(Point(16+dx, 22)); 
	this->vtStartText2.emplace_back(Point(17+dx, 23)); 
	this->vtStartText2.emplace_back(Point(18+dx, 24)); 

	this->vtStartText2.emplace_back(Point(20+dx, 20)); //K
	this->vtStartText2.emplace_back(Point(20+dx, 21));
	this->vtStartText2.emplace_back(Point(20+dx, 22));
	this->vtStartText2.emplace_back(Point(20+dx, 23));
	this->vtStartText2.emplace_back(Point(20+dx, 24));
	this->vtStartText2.emplace_back(Point(21+dx, 22));
	this->vtStartText2.emplace_back(Point(22+dx, 21));
	this->vtStartText2.emplace_back(Point(22+dx, 23));
	this->vtStartText2.emplace_back(Point(23+dx, 20));
	this->vtStartText2.emplace_back(Point(23+dx, 24));

	this->vtStartText2.emplace_back(Point(25+dx, 20)); //E
	this->vtStartText2.emplace_back(Point(25+dx, 21));
	this->vtStartText2.emplace_back(Point(25+dx, 22));
	this->vtStartText2.emplace_back(Point(25+dx, 23));
	this->vtStartText2.emplace_back(Point(25+dx, 24));
	this->vtStartText2.emplace_back(Point(26+dx, 20));
	this->vtStartText2.emplace_back(Point(26+dx, 22));
	this->vtStartText2.emplace_back(Point(26+dx, 24));
	this->vtStartText2.emplace_back(Point(27+dx, 20));
	this->vtStartText2.emplace_back(Point(27+dx, 22));
	this->vtStartText2.emplace_back(Point(27+dx, 24));
}

Animation::~Animation()
{
	if (!this->dqStartSnake.empty())
		this->dqStartSnake.clear();

	if (!this->vtStartText1.empty())
		this->vtStartText1.clear();

	if (!this->vtStartText2.empty())
		this->vtStartText2.clear();
}
	
void Animation::welcomeAnim()
{
	this->printStartSnake();
	this->moveStartSnake();
	this->printStartText1();
	this->printStartText2();
}

void Animation::printStartSnake()
{
	for (auto &point : this->dqStartSnake) {
		point.print();
		Sleep(this->m_nSpeed);
	}
}

void Animation::moveStartSnake()
{
	int nArrY[6] = { 13, 14, 15, 14, 13, 12 };

	// Snake moves from 10 to 50, then disappear the snake
	for (int x = 10, j = 0; x < 50; x++, j++) {
		int y = nArrY[j % 6];
		this->dqStartSnake.emplace_back(Point(x, y));
		this->dqStartSnake.back().print();
		this->dqStartSnake.front().clear();
		this->dqStartSnake.pop_front();
		Sleep(this->m_nSpeed);
	}

	while (!this->dqStartSnake.empty()) {
		this->dqStartSnake.front().clear();
		this->dqStartSnake.pop_front();
		Sleep(this->m_nSpeed);
	}
}

void Animation::printStartText1()
{
	while (vtStartText1.back().getX() < 45) {
		for (auto &point : this->vtStartText1) {
			if (point.getX() >= 0)
				point.clear();
			point.changePosition(point.getX() + 1, point.getY());
		}

		for (auto &point : this->vtStartText1)
			if (point.getX() >= 0)
				point.print();

		Sleep(this->m_nSpeed / 4);
	}
}

void Animation::printStartText2()
{
	while (vtStartText2.back().getX() < 36) {
		for (auto &point : this->vtStartText2) {
			if (point.getX() >= 0)
				point.clear();
			point.changePosition(point.getX() + 1, point.getY());
		}

		for (auto &point : this->vtStartText2)
			if (point.getX() >= 0)
				point.print();

		Sleep(this->m_nSpeed / 4);
	}
}
