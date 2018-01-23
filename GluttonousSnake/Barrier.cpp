#include "Barrier.h"
#include "DrawTool.h"

Barrier::Barrier()
{
}

Barrier::~Barrier()
{
	if (!this->m_vtBarriers.empty())
		this->m_vtBarriers.clear();
}

void Barrier::addBarrier(Point &pobj)
{
	this->m_vtBarriers.emplace_back(pobj);
}

vector<Point> Barrier::getBarriers()
{
	return this->m_vtBarriers;
}

void Barrier::clearAll()
{
	if (!this->m_vtBarriers.empty())
		this->m_vtBarriers.clear();
}

void Barrier::display()
{
	DrawTool::SetColor(FG_GRAY);
	for (auto &point : this->m_vtBarriers) {
		point.print();
	}
}
