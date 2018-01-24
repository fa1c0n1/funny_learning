#include "Barrier.h"
#include "DrawTool.h"

Barrier::Barrier()
{
}

Barrier::~Barrier()
{
	if (!this->m_vtBarriers.empty())
		this->m_vtBarriers.clear();

	if (!this->m_BarrierSet.empty())
		this->m_BarrierSet.clear();
}

void Barrier::addBarrier(Point &pobj)
{
	this->m_vtBarriers.emplace_back(pobj);
}

void Barrier::addCustomBarrier(Point &pobj)
{
	this->m_BarrierSet.insert(pobj);
}

void Barrier::delCustomBarrier(Point pobj)
{
	this->m_BarrierSet.erase(pobj);
}

vector<Point> Barrier::getBarriers()
{
	return this->m_vtBarriers;
}

set<Point, PointLess> Barrier::getCustomBarriers()
{
	return this->m_BarrierSet;
}

int Barrier::getBarrierLength()
{
	return this->m_vtBarriers.size();
}

int Barrier::getCustomBarrierLength()
{
	return this->m_BarrierSet.size();
}

void Barrier::clearAll()
{
	if (!this->m_vtBarriers.empty())
		this->m_vtBarriers.clear();
}

void Barrier::clearCustomAll()
{
	if (!this->m_BarrierSet.empty())
		this->m_BarrierSet.clear();
}

void Barrier::display()
{
	DrawTool::SetColor(FG_GRAY);

	if (this->m_BarrierSet.empty()) {
		for (auto &point : this->m_vtBarriers) {
			point.print();
		}
	}
	else {
		for (set<Point, PointLess>::iterator it = this->m_BarrierSet.begin(); 
			it != this->m_BarrierSet.end(); ++it) {
			Point point = *it;
			point.print();
		}
	}
}
