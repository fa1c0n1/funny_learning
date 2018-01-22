#pragma once

#include "Point.h"
#include "Comm.h"

#include <vector>

using namespace std;

class GameMap
{
public:
	GameMap(GameLevel level);
	~GameMap();
	void prnMap();
private:
	vector<Point> vtMap;
	GameLevel m_eGLevel;
	int m_nSideLen;
};

