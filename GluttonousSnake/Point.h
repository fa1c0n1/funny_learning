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
	int getX() const;
	int getY() const;
	void changePosition(const int x, const int y);
	bool operator==(Point &pobj);
		
private:
	int m_nX, m_nY;
};

struct PointLess
{
	bool operator() (const Point &p1, const Point &p2) const
	{
		if (p1.getX() < p2.getX())
			return true;
		else if (p1.getX() == p2.getX())
			return p1.getY() < p2.getY();
		else
			return false;
	}
};