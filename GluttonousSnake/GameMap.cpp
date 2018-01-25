#include "GameMap.h"
#include "DrawTool.h"

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

	// 初始化障碍物
	this->m_pBarrier = new Barrier;
}

GameMap::~GameMap()
{
	if (!this->vtMap.empty())
		this->vtMap.clear();

	if (this->m_pBarrier != nullptr) {
		delete this->m_pBarrier;
		this->m_pBarrier = nullptr;
	}
}

void GameMap::arrangeBarriers()
{
	//this->m_pBarrier->clearAll();

	if (this->m_pBarrier->getCustomBarrierLength() <= 0) {
		if (this->m_pBarrier->getBarrierLength() <= 0) {
			if (this->m_eGLevel == GL_NORMAL) {
				this->m_pBarrier->addBarrier(Point(3, 10));
				this->m_pBarrier->addBarrier(Point(3, 11));
				this->m_pBarrier->addBarrier(Point(3, 12));
				this->m_pBarrier->addBarrier(Point(3, 13));
				this->m_pBarrier->addBarrier(Point(3, 14));

				this->m_pBarrier->addBarrier(Point(7, 5));
				this->m_pBarrier->addBarrier(Point(8, 5));
				this->m_pBarrier->addBarrier(Point(8, 6));
				this->m_pBarrier->addBarrier(Point(8, 7));

				this->m_pBarrier->addBarrier(Point(12, 15));
				this->m_pBarrier->addBarrier(Point(12, 16));
				this->m_pBarrier->addBarrier(Point(13, 14));
				this->m_pBarrier->addBarrier(Point(13, 15));
				this->m_pBarrier->addBarrier(Point(14, 14));
				this->m_pBarrier->addBarrier(Point(15, 14));

				this->m_pBarrier->addBarrier(Point(5, 20));
				this->m_pBarrier->addBarrier(Point(5, 21));
				this->m_pBarrier->addBarrier(Point(6, 20));
				this->m_pBarrier->addBarrier(Point(6, 21));

				this->m_pBarrier->addBarrier(Point(30, 19));
				this->m_pBarrier->addBarrier(Point(30, 20));
				this->m_pBarrier->addBarrier(Point(30, 21));
				this->m_pBarrier->addBarrier(Point(30, 22));
				this->m_pBarrier->addBarrier(Point(30, 23));
				this->m_pBarrier->addBarrier(Point(30, 24));
				this->m_pBarrier->addBarrier(Point(30, 25));
				this->m_pBarrier->addBarrier(Point(30, 26));

				this->m_pBarrier->addBarrier(Point(9, 26));
				this->m_pBarrier->addBarrier(Point(10, 26));
				this->m_pBarrier->addBarrier(Point(11, 26));
				this->m_pBarrier->addBarrier(Point(12, 26));
				this->m_pBarrier->addBarrier(Point(12, 25));
				this->m_pBarrier->addBarrier(Point(12, 24));
				this->m_pBarrier->addBarrier(Point(13, 24));
				this->m_pBarrier->addBarrier(Point(14, 24));

				this->m_pBarrier->addBarrier(Point(21, 5));
				this->m_pBarrier->addBarrier(Point(21, 6));
				this->m_pBarrier->addBarrier(Point(21, 7));
				this->m_pBarrier->addBarrier(Point(22, 7));
				this->m_pBarrier->addBarrier(Point(23, 7));
				this->m_pBarrier->addBarrier(Point(23, 8));
				this->m_pBarrier->addBarrier(Point(23, 9));
			}
			else if (this->m_eGLevel == GL_HARD) {
				this->m_pBarrier->addBarrier(Point(5, 4));
				this->m_pBarrier->addBarrier(Point(5, 5));
				this->m_pBarrier->addBarrier(Point(5, 6));
				this->m_pBarrier->addBarrier(Point(5, 7));
				this->m_pBarrier->addBarrier(Point(6, 5));
				this->m_pBarrier->addBarrier(Point(7, 5));

				this->m_pBarrier->addBarrier(Point(11, 10));
				this->m_pBarrier->addBarrier(Point(12, 9));
				this->m_pBarrier->addBarrier(Point(12, 10));
				this->m_pBarrier->addBarrier(Point(12, 11));
				this->m_pBarrier->addBarrier(Point(12, 12));
				this->m_pBarrier->addBarrier(Point(13, 10));
				this->m_pBarrier->addBarrier(Point(14, 10));

				this->m_pBarrier->addBarrier(Point(18, 4));
				this->m_pBarrier->addBarrier(Point(19, 4));
				this->m_pBarrier->addBarrier(Point(20, 4));
				this->m_pBarrier->addBarrier(Point(20, 5));
				this->m_pBarrier->addBarrier(Point(21, 4));
				this->m_pBarrier->addBarrier(Point(21, 5));

				this->m_pBarrier->addBarrier(Point(25, 10));
				this->m_pBarrier->addBarrier(Point(26, 10));
				this->m_pBarrier->addBarrier(Point(27, 10));
				this->m_pBarrier->addBarrier(Point(27, 9));
				this->m_pBarrier->addBarrier(Point(27, 8));
				this->m_pBarrier->addBarrier(Point(27, 7));

				this->m_pBarrier->addBarrier(Point(28, 20));
				this->m_pBarrier->addBarrier(Point(29, 20));
				this->m_pBarrier->addBarrier(Point(29, 21));
				this->m_pBarrier->addBarrier(Point(29, 22));
				this->m_pBarrier->addBarrier(Point(29, 23));
				this->m_pBarrier->addBarrier(Point(29, 24));
				this->m_pBarrier->addBarrier(Point(29, 25));
				this->m_pBarrier->addBarrier(Point(29, 26));
				this->m_pBarrier->addBarrier(Point(28, 26));
				this->m_pBarrier->addBarrier(Point(27, 26));

				this->m_pBarrier->addBarrier(Point(19, 14));
				this->m_pBarrier->addBarrier(Point(20, 14));
				this->m_pBarrier->addBarrier(Point(19, 15));
				this->m_pBarrier->addBarrier(Point(20, 15));
				this->m_pBarrier->addBarrier(Point(19, 16));
				this->m_pBarrier->addBarrier(Point(20, 16));
				this->m_pBarrier->addBarrier(Point(19, 17));
				this->m_pBarrier->addBarrier(Point(20, 17));
				this->m_pBarrier->addBarrier(Point(19, 18));
				this->m_pBarrier->addBarrier(Point(20, 18));

				this->m_pBarrier->addBarrier(Point(4, 22));
				this->m_pBarrier->addBarrier(Point(4, 23));
				this->m_pBarrier->addBarrier(Point(4, 24));
				this->m_pBarrier->addBarrier(Point(4, 25));
				this->m_pBarrier->addBarrier(Point(4, 26));
				this->m_pBarrier->addBarrier(Point(5, 26));
				this->m_pBarrier->addBarrier(Point(6, 26));
				this->m_pBarrier->addBarrier(Point(6, 27));
				this->m_pBarrier->addBarrier(Point(6, 28));

				this->m_pBarrier->addBarrier(Point(9, 21));
				this->m_pBarrier->addBarrier(Point(10, 21));
				this->m_pBarrier->addBarrier(Point(11, 21));
				this->m_pBarrier->addBarrier(Point(12, 21));
				this->m_pBarrier->addBarrier(Point(13, 21));
				this->m_pBarrier->addBarrier(Point(14, 21));

				this->m_pBarrier->addBarrier(Point(11, 25));
				this->m_pBarrier->addBarrier(Point(12, 25));
				this->m_pBarrier->addBarrier(Point(11, 26));
				this->m_pBarrier->addBarrier(Point(12, 26));
				this->m_pBarrier->addBarrier(Point(13, 26));
				this->m_pBarrier->addBarrier(Point(14, 26));
				this->m_pBarrier->addBarrier(Point(15, 26));
				this->m_pBarrier->addBarrier(Point(11, 27));
				this->m_pBarrier->addBarrier(Point(12, 27));
				this->m_pBarrier->addBarrier(Point(13, 27));
				this->m_pBarrier->addBarrier(Point(14, 27));
				this->m_pBarrier->addBarrier(Point(15, 27));

				this->m_pBarrier->addBarrier(Point(4, 13));
				this->m_pBarrier->addBarrier(Point(4, 14));
				this->m_pBarrier->addBarrier(Point(4, 15));
				this->m_pBarrier->addBarrier(Point(5, 14));

				this->m_pBarrier->addBarrier(Point(12, 4));
				this->m_pBarrier->addBarrier(Point(13, 4));
			}
		}
	}
}

void GameMap::setBarrier(Barrier *pBarrier)
{
	if (this->m_pBarrier != nullptr) {
		delete this->m_pBarrier;
		this->m_pBarrier = nullptr;
		this->m_pBarrier = pBarrier;
	}
}

Barrier *GameMap::getBarrier()
{
	return this->m_pBarrier;
}

void GameMap::prnMap()
{
	DrawTool::SetColor(0);
	system("cls");
	//先把四周的墙打印出来
	DrawTool::SetColor(FG_LIGHTTURQUOISE);
	for (auto &point : this->vtMap) {
		point.print();
		Sleep(10);
	}
	
	//打印障碍物, 只有简单模式是没有障碍物的
	this->arrangeBarriers();
	this->m_pBarrier->display();
}

void GameMap::prnOnlyWall()
{
	system("cls");
	for (auto &point : this->vtMap) {
		point.print();
		Sleep(10);
	}
}

void GameMap::setGameLevel(GameLevel level)
{
	this->m_eGLevel = level;
}

GameLevel GameMap::getGameLevel()
{
	return this->m_eGLevel;
}

