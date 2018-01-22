#pragma once

#include "Point.h"

#include <deque>
#include <vector>

using namespace std;

class Animation
{
public:
	Animation();
	~Animation();
	void welcomeAnim();
private:
	void printStartSnake();
	void moveStartSnake();
	void printStartText1();
	void printStartText2();
	deque<Point> dqStartSnake;
	vector<Point> vtStartText2;
	vector<Point> vtStartText1;
	int m_nSpeed;
};

