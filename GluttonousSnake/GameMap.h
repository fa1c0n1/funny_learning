#pragma once

#include "Point.h"
#include "Comm.h"
#include "Barrier.h"

#include <vector>

using namespace std;

class GameMap
{
public:
	GameMap(GameLevel level);
	~GameMap();
	void prnMap();
	void prnOnlyWall();
	void setGameLevel(GameLevel level);
	GameLevel getGameLevel();
	void setBarrier(Barrier *pBarrier);
	Barrier *getBarrier();
private:
	void arrangeBarriers();

	vector<Point> vtMap;
	Barrier *m_pBarrier;
	GameLevel m_eGLevel;
	int m_nSideLen;
};

