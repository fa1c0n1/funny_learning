#include "Point.h"
#include "DrawTool.h"

#include <iostream>

using namespace std;


Point::Point(const int x, const int y)
	: m_nX(x), m_nY(y)
{
}

Point::Point(const Point &ptObj)
	: m_nX(ptObj.m_nX), m_nY(ptObj.m_nY)
{
}


Point::~Point()
{
}

void Point::print()
{
	DrawTool::SetCursorPosition(this->m_nX, this->m_nY);
	cout << "¡ö";
}

void Point::printCircle()
{
	DrawTool::SetCursorPosition(this->m_nX, this->m_nY);
	cout << "¡ñ";
}
	
void Point::clear()
{
	DrawTool::SetCursorPosition(this->m_nX, this->m_nY);
	cout << "  ";
}

void Point::setX(int x)
{
	this->m_nX = x;
}

void Point::setY(int y)
{
	this->m_nY = y;
}

int Point::getX() const
{
	return this->m_nX;
}

int Point::getY() const
{
	return this->m_nY;
}

void Point::changePosition(const int x, const int y)
{
	this->m_nX = x;
	this->m_nY = y;
}

bool Point::operator==(Point &pobj)
{
	return this->m_nX == pobj.m_nX && this->m_nY == pobj.m_nY;
}
