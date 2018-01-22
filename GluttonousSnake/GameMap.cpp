#include "GameMap.h"

#include <windows.h>

GameMap::GameMap(GameLevel level)
	: m_eGLevel(level), m_nSideLen(32)
{
	// 左边的墙
	for (int i = 1; i <= this->m_nSideLen; i++) {
		this->vtMap.emplace_back(Point(1, i));
	}

	// 上、下两边的墙
	for (int i = 2; i <= this->m_nSideLen; i++) {
		this->vtMap.emplace_back(Point(i, 1));
		this->vtMap.emplace_back(Point(i, 32));
	}

	// 右边的墙
	for (int i = 1; i <= this->m_nSideLen; i++) {
		this->vtMap.emplace_back(Point(this->m_nSideLen, i));
	}
}

GameMap::~GameMap()
{
	if (!this->vtMap.empty())
		this->vtMap.clear();
}

void GameMap::prnMap()
{
	for (auto &point : this->vtMap) {
		point.print();
		Sleep(10);
	}
}