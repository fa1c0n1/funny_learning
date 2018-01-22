#pragma once

class Point
{
public:
	Point(const int x, const int y);
	Point(const Point &ptObj);
	~Point();
	void print();
	void printCircle();
	void clear();
	void setX(int x);
	void setY(int y);
	int getX();
	int getY();
	void changePosition(const int x, const int y);
	bool operator==(Point &pobj);
		
private:
	int m_nX, m_nY;
};

