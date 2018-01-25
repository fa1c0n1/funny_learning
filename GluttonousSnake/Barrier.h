#pragma once

#include "Point.h"
#include "Comm.h"

#include <vector>
#include <set>

using namespace std;

class Barrier
{
public:
	Barrier();
	~Barrier();
	vector<Point> getBarriers();
	set<Point, PointLess> getCustomBarriers();
	void addBarrier(Point &pobj);
	void delBarrier(Point &pobj);
	void addCustomBarrier(Point &pobj);
	void delCustomBarrier(Point pobj);
	int getBarrierLength();
	int getCustomBarrierLength();
	void clearAll();
	void clearCustomAll();
	void display();
private:
	vector<Point> m_vtBarriers;
	set<Point, PointLess> m_BarrierSet;
};

