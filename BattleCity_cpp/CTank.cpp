#include "CTank.h"
#include "DrawTool.h"
#include "Data.h"
#include "CGameMap.h"
#include <vector>

using std::vector;

int CTank::m_nTankShape[4][3][3] = {
		{ //UP
			{ 0, 1, 0 },
			{ 1, 1, 1 },
			{ 1, 0, 1 }
		},
		{ //DOWN
			{ 1, 0, 1 },
			{ 1, 1, 1 },
			{ 0, 1, 0 }
		},
		{ //LEFT
			{ 0, 1, 1 },
			{ 1, 1, 0 },
			{ 0, 1, 1 }
		},
		{ //RIGHT
			{ 1, 1, 0 },
			{ 0, 1, 1 },
			{ 1, 1, 0 }
		}
};

CTank::CTank()
{
}

CTank::~CTank()
{
}

void CTank::drawObject()
{
	WORD wAttr = 0;
	switch (m_nType)
	{
	case SIGN_TANK_PA:
		wAttr = FG_LIGHTTURQUOISE;
		break;
	case SIGN_TANK_PB:
		wAttr = FG_LIGHTGREEN;
		break;
	case SIGN_TANK_E0:
		wAttr = FG_LIGHTRED;
		break;
	case SIGN_TANK_E1:
		wAttr = FG_LIGHTPURPLE;
		break;
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (m_pMap->getMapValue(m_nX+j, m_nY+i) != SIGN_GRASS)
				m_pMap->setMapValue(m_nX + j, m_nY + i, m_nType);
			if (m_nTankShape[m_nDrt][i][j] == 1) {
				if (m_pMap->getMapValue(m_nX+j, m_nY+i) != SIGN_GRASS)
					DrawTool::drawPattern(m_nX + j, m_nY + i, m_nType);
				else
					DrawTool::drawPattern(m_nX + j, m_nY + i, SIGN_GRASS);
			}
		}
	}
}

void CTank::clearObject()
{
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (m_pMap->getMapValue(m_nX + j, m_nY + i) != SIGN_GRASS) {
				m_pMap->setMapValue(m_nX + j, m_nY + i, SIGN_EMPTY);
				DrawTool::drawPattern(m_nX + j, m_nY + i, SIGN_EMPTY);
			}
		}
	}
}

CTank &CTank::getTankBirthPlace(int nType, int nEnemyNo)
{
	switch (nType)
	{
	case SIGN_TANK_PA:
		m_nDrt = DRT_UP;
		m_nX = 12;
		m_nY = 36;
		m_nType = SIGN_TANK_PA;
		break;
	case SIGN_TANK_PB:
		m_nDrt = DRT_UP;
		m_nX = 22;
		m_nY = 36;
		m_nType = SIGN_TANK_PB;
		break;
	case SIGN_TANK_E0:
	case SIGN_TANK_E1:
		m_nDrt = DRT_DOWN;
		m_nX = 5 + nEnemyNo * 5;
		m_nY = 1;
		m_nType = nType;
		break;
	}

	return *this;
}

void CTank::moveTank(int nDrt)
{
	//如果方向与原先不一致，先调整方向
	if (m_nDrt != nDrt) {
		clearObject();
		m_nDrt = nDrt;
		drawObject();
		return;
	}

	//判断坦克的碰撞
	if (isCollision())
		return;

	drawObject();
}

bool CTank::isCollision()
{
	bool bRet = false;

	switch (m_nDrt)
	{
	case DRT_UP:
		for (int i = 0; i < 3; i++) {
			if (m_pMap->getMapValue(m_nX + i, m_nY - 1) != SIGN_EMPTY && m_pMap->getMapValue(m_nX + i, m_nY - 1) != SIGN_GRASS) {
				bRet = true;
				break;
			}
		}

		if (!bRet) {
			clearObject();
			m_nY -= 1;
		}
		break;
	case DRT_DOWN:
		for (int i = 0; i < 3; i++) {
			if (m_pMap->getMapValue(m_nX + i, m_nY + 3) != SIGN_EMPTY && m_pMap->getMapValue(m_nX + i, m_nY + 3) != SIGN_GRASS) {
				bRet = true;
				break;
			}
		}

		if (!bRet) {
			clearObject();
			m_nY += 1;
		}
		break;
	case DRT_LEFT:
		for (int i = 0; i < 3; i++) {
			if (m_pMap->getMapValue(m_nX - 1, m_nY + i) != SIGN_EMPTY && m_pMap->getMapValue(m_nX - 1, m_nY + i) != SIGN_GRASS) {
				bRet = true;
				break;
			}
		}

		if (!bRet) {
			clearObject();
			m_nX -= 1;
		}
		break;
	case DRT_RIGHT:
		for (int i = 0; i < 3; i++) {
			if (m_pMap->getMapValue(m_nX + 3, m_nY + i) != SIGN_EMPTY && m_pMap->getMapValue(m_nX + 3, m_nY + i) != SIGN_GRASS) {
				bRet = true;
				break;
			}
		}

		if (!bRet) {
			clearObject();
			m_nX += 1;
		}
		break;
	}

	return bRet;
}

void CTank::setMapObj(CGameMap *pGMap)
{
	m_pMap = pGMap;
}

void CTank::setDead(bool bDead)
{
	m_bDead = bDead;
}

bool CTank::isDead()
{
	return m_bDead;
}

int CTank::getDirection()
{
	return m_nDrt;
}

int CTank::getPosX()
{
	return m_nX;
}

int CTank::getPosY()
{
	return m_nY;
}

int CTank::getType()
{
	return m_nType;
}
