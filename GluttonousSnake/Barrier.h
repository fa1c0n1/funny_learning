#pragma once

#include "Point.h"
#include "Comm.h"

#include <vector>

using namespace std;

class Barrier
{
public:
	Barrier();
	~Barrier();
	vector<Point> getBarriers();
	void addBarrier(Point &pobj);
	void clearAll();
	void display();
private:
	vector<Point> m_vtBarriers;
};

